#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//  #include "class/Vec2/Vec2.h"
#include "class/Vec2/Vec2.c"

//  #include "class/mapgen/mapgen.h"
#include "class/mapgen/mapgen.c"

//  #include "class/solver/solver.h"
#include "class/solver/solver.c"

//  #include "class/stack/stack.h"
#include "class/stack/stack.c"

#define MAXCHAR 10

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("\n[+] Usage: {height} {width} {difficulty} {filename}\n");
		printf("[+] Difficulty: 1 = easy, 2 = medium, 3 = hard\n");
		return EXIT_FAILURE;
	}

	int height = atoi(argv[1]),width = atoi(argv[2]);
	char export_location[9+MAXCHAR] = "exported/";
	strcat_s(export_location,sizeof(export_location),argv[4]);
	printf("\n%s",export_location);
	srand(time(NULL));
	
	Map* map = NULL;
	Vec2 size = { height, width };

	map = genMap(map, size, (Difficulty)atoi(argv[3]));

	printf("\n[*] Generated map path :");
	print_shard(map,&printPath);
	print_shard(map,&printMapData);
	export_map(map,export_location);
	
	Map* map_to_solve = NULL;
	map_to_solve = import_map(map_to_solve,export_location);
	Vec2 osef = {0,0};
	map_to_solve->path_count = 0;
	map_to_solve->path = (Vec2*)malloc(sizeof(Vec2)*50);

	printf("\n map to solve :");
	print_shard(map_to_solve,&printPath);
	print_shard(map_to_solve,&printMapData);
	stack_move(map,osef);

	if(Solve(map_to_solve,0,0,osef,0))
		printf("\nSolution found !");

	print_shard(map_to_solve,&printPath);
	draw_path(map_to_solve);
	print_shard(map_to_solve,&printPath);
	return EXIT_SUCCESS;

}
