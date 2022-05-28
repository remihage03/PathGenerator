#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include "graph.h"
//#include "node.h"


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
	// export_map(map, argv[4]);

	// Map* map2 = NULL;
	// map2 = import_map(map2, argv[4]);
	// megaSolver3000(map2);
	// Stack* path;
	// int res = map->size.x * map->size.y;
	// NewStack(&path, res);

	//Vec2 entry = map->entry;
	//Node* node = NULL;
	//printf("\ncreating graph..");
	//// create_node(node,entry,map,DIR_LEFT);
	//node = create_master_node(node,entry,map);
	//for(int i = 0;i<4;i++){
	//	create_children(node,map,i);
	//}
	//printf("\n graph done ..");
	//
	//explore_graph(node);

	int res = (map->size.x - 2) * (map->size.y - 2);
	graph g = newGraph(res, false);

	for (int i = 1; i < map->size.x - 1; i++)
	{
		for (int j = 1; j < map->size.y - 1; j++)
		{
			addEdge(g, i, j);
		}
	}

	printGraph(g);

	destroyGraph(g);

	return EXIT_SUCCESS;
}