#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
//#include "mapgen.c"


int main()
{
	srand(time(NULL));
	Map* map = NULL;
	Vec2 size = { 31, 31 };

	map = genMap(map, size, DIFF_EASY);
	print_shard(map,&printPath);
	//print_shard(map,&printMapData);

}