#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
//#include "mapgen.c"

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("\n[+] Usage: {height} {width} {difficulty} {filename}\n");
		printf("[+] Difficulty: 1 = easy, 2 = medium, 3 = hard\n");
		return EXIT_FAILURE;
	}
	srand(time(NULL));
	Map* map = NULL;
	Vec2 size = { atoi(argv[1]), atoi(argv[2]) };
	map = genMap(map, size, (Difficulty)atoi(argv[3]));
	// print_shard(map,&printPath);
	// print_shard(map,&printMapData);
	exportMap(map, argv[4]);
	Map* map2 = NULL;
	map2 = parseJson(map2,argv[4]);
	if(map2){
		printMapInfo(map2);
		print_shard(map2,&printMapData);
	}
	
	print_shard(map, &printPath);
	solver(map);
	//print_shard(map, &printPath);

	// printf("%d", calcDist(map->entry, map->exit));
	return EXIT_SUCCESS;
}