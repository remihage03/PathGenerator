#pragma once
#include <stdbool.h>

typedef struct node {
	Vec2 pos;
	unsigned int g_cost, h_cost, f_cost;
}Node;


unsigned int ManDist(Vec2 a, Vec2 b);

bool isValid(Map* map, Vec2 pos);

Node getNodeFromMap(Map* map, Vec2 pos);

int megaSolver2000(Map* map);