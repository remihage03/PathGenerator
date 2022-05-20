<<<<<<< HEAD
=======
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
>>>>>>> e4d0e7a38533644f3669aa84ba58b3ce0778cfe0
