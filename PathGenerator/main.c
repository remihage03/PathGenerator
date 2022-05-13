#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"


int main()
{
	srand(time(NULL));
	Map* map = NULL;
	Vec2 size = { 15, 15 };

	map = genMap(map, size, DIFF_EASY);
	printMap(map);
}