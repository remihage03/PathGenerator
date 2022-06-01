#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "mapgen.c"
#include "solver.h"
#include "solver.c"
#include "stack.h"
#include "stack.c"

#include "graph_sacha.c"
#include "graph_sacha.h"

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("\n[+] Usage: {height} {width} {difficulty} {filename}\n");
		printf("[+] Difficulty: 1 = easy, 2 = medium, 3 = hard\n");
		return EXIT_FAILURE;
	}

	srand(time(NULL));
	int total20 = 0;
	int total21 = 0;
	
	int ite = 10000;
	Map* map = NULL;
	Map* map2 = NULL;
	Vec2 size = { 21, 21 };
	Vec2 size2 = { 22, 22 };

	map = genMap(map, size, (Difficulty)atoi(argv[3]));

	print_shard(map,&printPath);
	export_map(map,"file.json");

	printf("\n20 : %d,21 %d <=> en ratio : 20 = %f, 21 = %f",total20,total21,(float)total20/(float)ite,(float)total21/(float)ite);
	printf("\naugmentation : %f pourcents sur %d iterations",(float)total21/(float)total20,ite);
	
	
	Map* map_to_solve = NULL;
	map_to_solve = import_map(map_to_solve,"file.json");
	printf("\nnew map : \n");
	print_shard(map_to_solve,&printPath);
	print_shard(map_to_solve,&printMapData);
	int** map_content = map_to_solve->data;

	printf("\n pos : (1,1) = %d ; (2,1) = %d",map_content[1][1],map_content[2][1]);
	Vec2 osef = {0,0};

	map_to_solve->path_count = 0;
	map_to_solve->path = (Vec2*)malloc(sizeof(Vec2)*50);
	printf("mapcount %d",map_to_solve->path_count);
	printf("\n return value : %d",Solve(map_to_solve,1,1,osef));

	print_shard(map_to_solve,&printPath);
	// for(int i =0;i<map_to_solve->path_count;i++){
	// 	printf("\n x : %d - y : %d ",map_to_solve->path[i].x,map_to_solve->path[i].y);
	// }
	draw_path(map_to_solve);
	print_shard(map_to_solve,&printPath);

	// int res = map->size.x * map->size.y;
	// graph* g = create_graph(res);

	// for (int y = 1; y < map->size.y - 1; y++)
	// {
	// 	for (int x = 1; x < map->size.x - 1; x++)
	// 	{
	// 		Vec2 pos = {x, y};
	// 		create_neighbor_list(g, map, pos);
	// 	}
	// }

	// export_map(map, argv[4]);

	// print_graph(g);
	// destroy_graph(g);

	return EXIT_SUCCESS;

}
