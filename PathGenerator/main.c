#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include "node.h"
//#include "solver.c"
//#include "stack.c"
//#include "mapgen.c"
//#include "node.c"


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
	print_shard(map,&printPath);
	// print_shard(map,&printMapData);
	export_map(map, argv[4]);

	Map* map2 = NULL;
	map2 = import_map(map2, argv[4]);
	// megaSolver3000(map2);
	Stack* path;
	int res = map->size.x * map->size.y;
	NewStack(&path, res);
	Vec2 entry = map->entry;

	Node* node = NULL;
	printf("\ncreating graph..");
	create_node(&node,entry,map);
	printf("\n graph done ..");
	
	explore_graph(node);

	// megaSolver4000(map,path,entry);
	// for (int i = 1; i < map->size.x - 1; i++)
	// {
	// 	for (int j = 1; j < map->size.y - 1; j++)
	// 	{
	// 		Vec2 pos = { i, j };
	// 		int* cell = &map->data[i][j];
	// 		if (*cell != T_WALL && *cell != D_ROCK)
	// 			*cell = ManDist(pos, map->exit);
	// 	}
	// }
	// print_shard(map, &printMapData);

	return EXIT_SUCCESS;
}