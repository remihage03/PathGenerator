#pragma once
 #include "../mapgen/mapgen.h"
 #include "../stack/stack.h"
 #include "../Vec2/Vec2.h"

bool Solve(Map* map,int i, int j,Vec2 _from_pos);
void draw_path(Map* map);
int _2d_to_1d(Map* map, int x, int y);
