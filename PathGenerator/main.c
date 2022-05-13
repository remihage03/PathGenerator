#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
//#include "mapgen.c"

int main()
{
	srand(time(NULL));
	Map* map = NULL;
	Vec2 size = { 21, 21 };

	map = genMap(map, size, DIFF_EASY);
	print_shard(map,&printPath);
	print_shard(map,&printMapData);
	exportMap(map, "level.ice");

	/*printf("\n");
	for (int y = map->size.y / 2 - 1; y <= map->size.y / 2 + 1; y++)
	{
		for (int x = map->size.x / 2 - 1; x <= map->size.x / 2 + 1; x++)
		{
			Vec2 currPos = { x, y };
			if (checkPos(map, currPos))
				if ((currPos.x == map->size.x / 2 || currPos.y == map->size.y / 2))
					printf("%d ", map->data[currPos.x][currPos.y]);
		}
		printf("\n");
	}*/

}