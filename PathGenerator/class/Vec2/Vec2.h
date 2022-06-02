#include <stdbool.h>
#include <stdio.h>

typedef struct vec2 {
	int x, y;
}Vec2;

bool isEqual(Vec2 a, Vec2 b);
unsigned int ManDist(Vec2 a, Vec2 b); // Distance de manhattan
void swap_vec2(Vec2* va,Vec2* vb);