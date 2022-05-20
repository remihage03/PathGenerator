#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "mapgen.c"
// #include "solver.h"
#include "solver.c"
#include "stack.c"

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
	print_shard(map,&printMapData);
	export_map(map, argv[4]);

	megaSolver3000(map);
	return EXIT_SUCCESS;
}