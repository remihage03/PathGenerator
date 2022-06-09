#pragma once
// Initialize the generator from a seed
void seed_mt(int seed);

// Extract a tempered value based on MT[index]
// calling twist() every n numbers
int extract_number();

// Generate the next n values from the series x_i 
void twist();

// Create a length n array to store the state of the generator
void init_twister();

int twister_wrapped(int seed);