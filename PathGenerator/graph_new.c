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