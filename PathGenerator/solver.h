#pragma once
#include "mapgen.h"
#include <stdbool.h>

typedef struct node {
	Vec2 pos;
	unsigned int g_cost, h_cost, f_cost;
	bool walkable;
	struct node* parent;
}Node;

int megaSolver3000(Map* map);
unsigned int ManDist(Vec2 a, Vec2 b);