#pragma once
#include <stdbool.h>
#include "mapgen.h"

// Struct noeud
typedef struct NodeListElement 
{
	int value;
	struct Node* next;
}nodeListElement, *nodeList;

// Elts adjacents
typedef struct AdjencyListElement
{
	nodeListElement* begin;
}adjencyListElement, *adjencyList;

// Struct graph
typedef struct GraphElement
{
	bool isOriented;
	int nbVertices;
	adjencyList neighbors;
}graphElement, *graph;

graph newGraph(int vertices, bool isOriented);
bool isEmpty(graph g);
nodeList addNode(int x);
void addEdge(graph g, int src, int dest);
void printGraph(graph g);
void destroyGraph(graph g);

int addCellToGraph(graph g, Map* map, Vec2 pos);