#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph_new.h"

graph* create_graph(int numNodes)
{
	graph* g = malloc(sizeof(*g));

	if (g == NULL)
	{
		free(g);
		return NULL;
	}
	g->numNodes = numNodes;
	g->edges = calloc(sizeof(bool*), g->numNodes);

	if (g->edges == NULL)
	{
		free(g);
		return NULL;
	}

	for (int i = 0; i < g->numNodes; i++)
	{
		g->edges[i] = calloc(sizeof(bool), g->numNodes);
		if (g->edges[i] == NULL)
		{
			destroy_graph(g);
			return NULL;
		}
	}

	return g;
}

void destroy_graph(graph* g)
{
	if (g == NULL) return;

	for (int i = 0; i < g->numNodes; i++)
	{
		if (g->edges[i] != NULL)
			free(g->edges[i]);
	}
	free(g->edges);
	free(g);
}

void print_graph(graph* g)
{
	printf("digraph {\n");
	
	for (int from = 0; from < g->numNodes; from++)
	{
		for (int to = 0; to < g->numNodes; to++)
		{
			if (g->edges[from][to])
				printf("%d -> %d;\n", from, to);
		}
	}
	printf("}\n");
}

bool add_edge(graph* g, unsigned int from, unsigned int to)
{
	assert(g != NULL);
	assert(from < g->numNodes);
	assert(to < g->numNodes);

	if (has_edge(g, from, to)) return false;

	g->edges[from][to] = true;

	return true;
}

bool has_edge(graph* g, unsigned int from, unsigned int to)
{
	assert(g != NULL);
	assert(from < g->numNodes);
	assert(to < g->numNodes);

	return g->edges[from][to];
}


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
