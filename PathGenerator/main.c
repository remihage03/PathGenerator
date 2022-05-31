#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include "graph_new.h"
//#include "node.h"

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
	Map* map = NULL;
	Vec2 size = { atoi(argv[1]), atoi(argv[2]) };
	map = genMap(map, size, (Difficulty)atoi(argv[3]));
	print_shard(map,&printPath);
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

	return EXIT_SUCCESS;
}