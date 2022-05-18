#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define DEBUG

isInStack(Stack* stack, Node node)
{
	for (int i = 0; i < stack->eltsCount; i++)
	{
		if (stack->array[i].pos.x == node.pos.x && stack->array[i].pos.y == node.pos.y)
			return true;
	}
	return false;
}

unsigned int calcDist(Vec2 a, Vec2 b)
{
	if (a.x == b.x && a.y == b.y) return 0;
	// * 10 pour avoir plus de précision (en gros 2 chiffre après la virgule mais sans virgule)
	return (unsigned int)(10 * sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

bool isValid(Map* map, Vec2 pos)
{
	return (pos.x >= 0 && pos.x < map->size.x && pos.y >= 0 && pos.y < map->size.y);
}

Node getNodeFromMap(Map* map, Vec2 pos)
{
	if (!map || !isValid(map, pos)) return;

	Node tmp;
	tmp.pos = pos;
	tmp.g_cost = calcDist(pos, map->entry);
	tmp.h_cost = calcDist(pos, map->exit);
	tmp.f_cost = tmp.g_cost + tmp.h_cost;

	return tmp;
}

void printNode(Node node)
{
	printf("\n[+] Node:\nPos: (%d, %d)\nG_cost: %d\nH_cost: %d\nF_cost: %d\n", node.pos.x, node.pos.y, node.g_cost, node.h_cost, node.f_cost);
}

int megaSolver2000(Map* map)
{
	if (!map) return ERROR;

	// Stack of nodes to be evaluated
	Stack* open;
	NewStack(&open, map->size.x * map->size.y);


	// Stack of nodes already evaluated
	Stack* closed;
	NewStack(&closed, map->size.x * map->size.y);

	Node startNode = getNodeFromMap(map, map->entry);
	Node endNode = getNodeFromMap(map, map->exit);

	push(open, startNode);

#ifdef DEBUG
	printNode(startNode);
	printNode(endNode);
#endif // DEBUG

	Node curr = startNode;
	while (!isStackEmpty(open))
	{
		Node bestNode = curr;
		unsigned int lowest = INT_MAX;
		for (int i = 0; i < open->eltsCount; i++)
		{
			curr = open->array[i];
			if (curr.f_cost < lowest)
			{
				lowest = curr.f_cost;
				bestNode = curr;
			}
		}

#ifdef DEBUG
		printf("\nBest node:");
		printNode(bestNode);
#endif // DEBUG


		pull(open, &bestNode);
		push(closed, bestNode);

		if (bestNode.pos.x == endNode.pos.x && bestNode.pos.y == endNode.pos.y)
			return;

		// Get Node neighbors
		Node neighbors[4];
		neighbors[0] = getNodeFromMap(map, (Vec2) { bestNode.pos.x, bestNode.pos.y - 1 });
		neighbors[1] = getNodeFromMap(map, (Vec2) { bestNode.pos.x + 1, bestNode.pos.y });
		neighbors[2] = getNodeFromMap(map, (Vec2) { bestNode.pos.x, bestNode.pos.y + 1 });
		neighbors[3] = getNodeFromMap(map, (Vec2) { bestNode.pos.x - 1, bestNode.pos.y });

#ifdef DEBUG
		printf("\nBest Node neighbors: ");
		for (size_t i = 0; i < 4; i++)
		{
			printNode(neighbors[i]);
		}
#endif // DEBUG

		Node closest = bestNode;
		for (int i = 0; i < 4; i++)
		{
			Vec2 pos = neighbors[i].pos;
			int cell = map->data[pos.x][pos.y];
			
			// Is the node traversable ?
			if ((cell != 1 && cell != 0) || isInStack(closed, neighbors[i]))
				continue;

			// Geting the closest node
			if (neighbors[i].f_cost <= closest.f_cost || !isInStack(open, neighbors[i]))
				closest = neighbors[i];
		}
		if (!isInStack(open, closest))
			push(open, closest), curr = closest;

#ifdef DEBUG
		printf("\nMoving to:");
		printNode(closest);
#endif // DEBUG
	}


	return SUCCESS;
}