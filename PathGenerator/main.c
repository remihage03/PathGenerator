#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

//   #include "class/Vec2/Vec2.h"
#include "class/Vec2/Vec2.c"

//   #include "class/mapgen/mapgen.h"
#include "class/mapgen/mapgen.c"

//   #include "class/solver/solver.h"
#include "class/solver/solver.c"

//   #include "class/stack/stack.h"
#include "class/stack/stack.c"

#define MAXCHAR 10

int main(int argc, char* argv[])
{
	srand(time(NULL));

	if (argc != 6)
	{
		printf("\n[+] Usage: {mode} {height} {width} {difficulty} {filename}\n");
		printf("[+] Modes: solve, gen\n");
		printf("[+] Difficulty: 1 = easy, 2 = medium, 3 = hard\n");
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "solve") && strcmp(argv[1], "gen"))
	{
		printf("[+] Incorrect mode !\n");
		printf("[+] Modes: solve, gen\n");
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "solve") == 0)
	{
		Map* map_to_solve = NULL;
		char export_location[9 + MAXCHAR] = "exported/";
		strcat_s(export_location, sizeof(export_location), argv[5]);
		map_to_solve = import_map(map_to_solve, export_location);
		Vec2 osef = { 0,0 };
		map_to_solve->path_count = 0;
		map_to_solve->path = (Vec2*)malloc(sizeof(Vec2) * 50);

		printf("\n map to solve :");
		print_shard(map_to_solve, &printPath);
		print_shard(map_to_solve, &printMapData);

		if (Solve(map_to_solve, 0, 0, osef))
			printf("\nSolution found !");

		// print_shard(map_to_solve,&printPath);
		draw_path(map_to_solve);
		print_shard(map_to_solve, &printPath);
	}
	else
	{
		int height = atoi(argv[2]), width = atoi(argv[3]);
		char export_location[9 + MAXCHAR] = "exported/";
		strcat_s(export_location, sizeof(export_location), argv[5]);
		printf("\n%s", export_location);

		Map* map = NULL;
		Vec2 size = { height, width };

		map = genMap(map, size, (Difficulty)atoi(argv[4]));

		printf("\n[*] Generated map path :");
		print_shard(map, &printPath);
		print_shard(map, &printMapData);
		export_map(map, export_location);
	}
	return EXIT_SUCCESS;
}
