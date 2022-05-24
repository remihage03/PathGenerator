#pragma once
#include "mapgen.h"
#include <stdbool.h>
#include "stack.h"


int megaSolver3000(Map* map);
//void move_until_rock(Map* map, Stack* path, Vec2* pos, Dir dir, bool simulation);
bool isWalkable(Map* map, Vec2 pos);
bool canMove(Map* map, Vec2 pos, Dir dir);
bool isEqual(Vec2 a, Vec2 b);
unsigned int ManDist(Vec2 a, Vec2 b); // Distance de manhattan