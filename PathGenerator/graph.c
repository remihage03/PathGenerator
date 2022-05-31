#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

graph newGraph(int vertices, bool isOriented)
{
	graphElement* elt = malloc(sizeof(*elt));

	if (elt == NULL)
	{
		fprintf(stderr, "Error while creating graph\n");
		exit(EXIT_FAILURE);
	}

	elt->isOriented = isOriented;
	elt->nbVertices = vertices;

	elt->neighbors = malloc(vertices * sizeof(adjencyListElement));

	if (elt->neighbors == NULL)
	{
		fprintf(stderr, "Error while creating neighbors array\n");
		exit(EXIT_FAILURE);
	}

	for (int ctr = 0; ctr < elt->nbVertices; ctr++)
		elt->neighbors[ctr].begin = NULL;

	return elt;
}


bool isEmpty(graph g)
{
	return (g == NULL);
}


nodeList addNode(int x)
{
	nodeList n = malloc(sizeof(nodeListElement));

	if (n == NULL)
	{
		fprintf(stderr, "Error while creating node\n");
		exit(EXIT_FAILURE);
	}

	n->value = x;
	n->next = NULL;
}



void addEdge(graph g, int src, int dest)
{
	nodeList n = addNode(dest);
	n->next = g->neighbors[src].begin;
	g->neighbors[src].begin = n;

	if (!g->isOriented) // graph unidirectionnel
	{
		n = addNode(src);
		n->next = g->neighbors[dest].begin;
		g->neighbors[dest].begin = n;
	}
}



void printGraph(graph g)
{
	if (!g->nbVertices) return;

	printf("digraph {\n");

	//for (int i = 0; i < g->nbVertices; i++)
	//{
	//	for (int j = 0; j < g->nbVertices; j++)
	//	{
	//		if (g->neighbors[i].begin)
	//			printf("%d -> %d;\n", i, j);
	//	}
	//}

	for (int i = 0; i < g->nbVertices; i++)
	{
		nodeList n = g->neighbors[i].begin;
		//printf("%d -> ", i);
		while (n != NULL)
		{
			printf("%d -> %d;\n", i, n->value);
			//printf("%d;\n", n->value);
			n = n->next;
		}
		//printf(";\n");
	}
	printf("}\n");
}



void destroyGraph(graph g)
{
	if (isEmpty(g)) return;

	// Si sommets adjacents
	if (g->neighbors)
	{
		for (int ctr = 0; ctr < g->nbVertices; ctr++)
		{
			nodeList n = g->neighbors[ctr].begin;
			
			while (n != NULL)
			{
				nodeList tmp = n;
				n = n->next;
				free(tmp);
			}
		}
		// free liste adjacents
		free(g->neighbors);
	}
	// free graph
	free(g);
}

int addCellToGraph(graph g, Map* map, Vec2 pos)
{
	if (!map) return ERROR;

	int cell = map->data[pos.x][pos.y];

	if (cell == T_WALL || cell == D_ROCK) return ERROR;

	if (map->data[pos.x - 1][pos.y] != T_WALL && map->data[pos.x - 1][pos.y] != D_ROCK)
		addEdge(g, pos.x, pos.x - 1);

	if (map->data[pos.x + 1][pos.y] != T_WALL && map->data[pos.x + 1][pos.y] != D_ROCK)
		addEdge(g, pos.x, pos.x + 1);

	if (map->data[pos.x][pos.y - 1] != T_WALL && map->data[pos.x][pos.y - 1] != D_ROCK)
		addEdge(g, pos.x, pos.y - 1);

	if (map->data[pos.x][pos.y + 1] != T_WALL && map->data[pos.x][pos.y + 1] != D_ROCK)
		addEdge(g, pos.x, pos.y + 1);

	return SUCCESS;
}