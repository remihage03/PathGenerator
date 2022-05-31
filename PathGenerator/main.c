#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include "node.h"
#include "solver.c"
#include "stack.c"
#include "mapgen.c"
#include "node.c"


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

	for(int i = 0;i<ite;i++){
		Map* map = NULL;
		Map* map2 = NULL;
		Vec2 size = { 21, 21 };
		Vec2 size2 = { 22, 22 };

		map = genMap(map, size, (Difficulty)atoi(argv[3]));
		map2 = genMap(map2, size2, (Difficulty)atoi(argv[3]));
		total20+= map->tournant;
		total21+= map2->tournant;
		// print_shard(map,&printPath);
	}
	printf("\n20 : %d,21 %d <=> en ratio : 20 = %f, 21 = %f",total20,total21,(float)total20/(float)ite,(float)total21/(float)ite);
	printf("\naugmentation : %f pourcents sur %d iterations",(float)total21/(float)total20,ite);
	return EXIT_SUCCESS;

}

	// Vec2 size2 = { atoi(argv[1]), atoi(argv[2]) };
	// print_shard(map,&printMapData);
	// export_map(map, argv[4]);

	// Map* map2 = NULL;
	// map2 = import_map(map2, argv[4]);
	// megaSolver3000(map2);
	// Stack* path;
	// int res = map->size.x * map->size.y;
	// NewStack(&path, res);

	// Vec2 entry = map->entry;
	// Node* node = NULL;
	// printf("\ncreating graph..");
	// // create_node(node,entry,map,DIR_LEFT);
	// node = create_master_node(node,entry,map);
	// for(int i = 0;i<4;i++){
	// 	create_children(node,map,i);
	// }
	// printf("\n graph done ..");

	// explore_graph(node);