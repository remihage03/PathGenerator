#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include "graph_new.h"
#include "node.h"

create_neighbor_list(graph* g, Map* map, Vec2 pos)
{
	Vec2 neighbors[999];
	Vec2 _pos = pos;
	int ctr = 0;
	while (map->data[_pos.x][pos.x ] != D_ROCK && map->data[_pos.x][pos.x] != T_WALL && isValid(map, _pos))
	{
		_pos.x++;
		neighbors[ctr] = _pos;
		ctr++;
	}
	_pos = pos;
	while (map->data[_pos.x][pos.x] != D_ROCK && map->data[_pos.x][pos.x] != T_WALL && isValid(map, _pos))
	{
		_pos.x--;
		neighbors[ctr] = _pos;
		ctr++;
	}

	while (map->data[_pos.x][pos.x] != D_ROCK && map->data[_pos.x][pos.x] != T_WALL && isValid(map, _pos))
	{
		_pos.y++;
		neighbors[ctr] = _pos;
		ctr++;
	}
	_pos = pos;
	while (map->data[_pos.x][pos.x] != D_ROCK && map->data[_pos.x][pos.x] != T_WALL && isValid(map, _pos))
	{
		_pos.y--;
		neighbors[ctr] = _pos;
		ctr++;
	}

	for (int i = 0; i < ctr; i++)
	{
		add_edge(g, (pos.y * map->size.y) + pos.x , (neighbors[i].y * map->size.y) +neighbors[i].x);
	}
}


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
	export_map(map, argv[4]);

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

	int res = map->size.x * map->size.y;
	graph* g = create_graph(res);

	for (int y = 1; y < map->size.y - 1; y++)
	{
		for (int x = 1; x < map->size.x - 1; x++)
		{
			Vec2 pos = {x, y};
			create_neighbor_list(g, map, pos);
		}
	}

	print_graph(g);
	destroy_graph(g);

	// explore_graph(node);