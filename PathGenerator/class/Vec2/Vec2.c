#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Vec2.h"

bool isEqual(Vec2 a, Vec2 b)
{
	return (a.x == b.x && a.y == b.y);
}

unsigned int ManDist(Vec2 a, Vec2 b)
{
	// * 10 pour avoir plus de précision (en gros 1 chiffre après la virgule mais sans virgule)
	return (unsigned int)(10 * (abs(b.x - a.x) + abs(b.y - a.y))); // Distance Manhattan
}


void swap_vec2(Vec2* va,Vec2* vb){
    Vec2 temp = *va;
    *va = *vb;
    *vb = temp;
}