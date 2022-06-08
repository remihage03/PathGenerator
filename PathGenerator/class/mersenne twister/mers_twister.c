#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mers_twister.h"

#if INTPTR_MAX == INT64_MAX // 64-bit
unsigned int w = 64, n = 312, m = 156, r = 31;
unsigned int a = 0xB5026F5AA96619E9;
unsigned int u = 29, d = 0x5555555555555555;
unsigned int s = 17, b = 0x71D67FFFEDA60000;
unsigned int t = 37, c = 0xFFF7EEE000000000;
unsigned int l = 43;
unsigned int f = 6364136223846793005;
#define sz_MT 312
#elif INTPTR_MAX == INT32_MAX // 32-bit
unsigned int w = 32, n = 624, m = 397, r = 31;
unsigned int a = 0x9908B0DF;
unsigned int u = 11, d = 0xFFFFFFFF;
unsigned int s = 7, b = 0x9D2C5680;
unsigned int t = 15, c = 0xEFC60000;
unsigned int l = 18;
unsigned int f = 1812433253;
#define sz_MT 624
#else
#error Unknown pointer size or missing size macros!
#endif



int MT[sz_MT]; //n
int index = sz_MT + 1;
int lower_mask;
int upper_mask;

// Initialize the generator from a seed
void seed_mt(int seed) {
    int index = n;
    MT[0] = seed;
    for (int i = 1; i < n; i++) { // loop over each element
        int temp = f * (MT[i - 1] ^ (MT[i - 1] >> (w - 2))) + i;
        MT[i] = temp & 0xffffffff;
    }
}

// Extract a tempered value based on MT[index]
// calling twist() every n numbers
int extract_number() {
    if (index >= n )
    {
        if (index > n) 
        {
          perror("Generator was never seeded");
          // Alternatively, seed with constant value; 5489 is used in reference C code[53]
        }
        twist();
    }

    int y = MT[index];
    y = y ^ ((y >> u) & d);
    y = y ^ ((y << s) & b);
    y = y ^ ((y << t) & c);
    y = y ^ (y >> l);

    index++;
    return y & 0xffffffff;
}

// Generate the next n values from the series x_i 
void twist() 
{
    //for (int i = 0; i < n; i++)
    //{
    //    int x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
    //    int xA = x >> 1;
    //    if ((x % 2) != 0) { // lowest bit of x is 1
    //        xA = xA ^ a;
    //    }
    //    MT[i] = MT[(i + m) % n] ^ xA;
    //}
    //index = 0;

    for (int i = 0; i < n; i++) 
    {
        int x = (MT[i] & upper_mask) + (MT[(i + 1) % n] & lower_mask);
        int xA = x >> 1;
        if ((x % 2) != 0)
            xA = xA ^ a;
        MT[i] = MT[(i + m) % n] ^ xA;
    }
    index = 0;
}

// Create a length n array to store the state of the generator
void init_twister()
{
    lower_mask = (1 << r) - 1; //0x7FFFFFFF;  // That is, the binary number of r 1's
    upper_mask = !lower_mask & 0xffffffff; // 0x80000000; //lowest w bits of(not lower_mask);
}


int twister_wrapped(int seed)
{
    init_twister();
    seed_mt(seed);
    twist();
    return extract_number();
}