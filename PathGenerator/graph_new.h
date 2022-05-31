#include <stdbool.h>
#include <stdlib.h>
#include "solver.h"

typedef struct Graph
{
	int numNodes;
	bool** edges;
}graph;

graph* create_graph(int numNodes);
void destroy_graph(graph* g);
void print_graph(graph* g);
bool add_edge(graph* g, unsigned int from, unsigned int to);
bool has_edge(graph* g, unsigned int from, unsigned int to);
create_neighbor_list(graph* g, Map* map, Vec2 pos);