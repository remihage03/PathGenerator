#include "mapgen.h"
#include "solver.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DEBUG

bool isInStack(Stack* stack, Node node)
{
	for (int i = 0; i < stack->eltsCount; i++)
	{
		if (stack->array[i].pos.x == node.pos.x && stack->array[i].pos.y == node.pos.y)
			return true;
	}
	return false;
}

void pullNode(Stack* stack, Node node)
{
	int idx = 0;
	bool found = false;
	for (idx; idx < stack->eltsCount; idx++)
	{
		if (stack->array[idx].pos.x == node.pos.x && stack->array[idx].pos.y == node.pos.y)
		{
			found = true;
			break;
		}
	}

	if (!found) return;

	for (int i = idx; i < stack->size - 1; i++)
	{
		stack->array[i] = stack->array[i + 1];
	}

	/* Decrement array size by 1 */
	stack->eltsCount--;
}

unsigned int ManDist(Vec2 a, Vec2 b)
{
	if (a.x == b.x && a.y == b.y) return 0;
	// * 10 pour avoir plus de précision (en gros 1 chiffre après la virgule mais sans virgule)
	return (unsigned int)(10 * (abs(b.x - a.x) + abs(b.y - a.y))); // Distance Manhattan
}

unsigned int EuclDist(Vec2 a, Vec2 b)
{
	if (a.x == b.x && a.y == b.y) return 0;
	// * 10 pour avoir plus de précision (en gros 1 chiffre après la virgule mais sans virgule)
	return (unsigned int)(10 * sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2))); // Distance Euclidienne
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
	tmp.g_cost = ManDist(pos, map->entry); //10;
	tmp.h_cost = EuclDist(pos, map->exit);
	tmp.f_cost = tmp.g_cost + tmp.h_cost;

	return tmp;
}

void printNode(Node node)
{
	printf("\n[+] Node:\nPos: (%d, %d)\nG_cost: %d\nH_cost: %d\nF_cost: %d\n", node.pos.x, node.pos.y, node.g_cost, node.h_cost, node.f_cost);
}

// Algorithme A*
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
	printf("\nStart node:");
	printNode(startNode);

	printf("\nEnd node:");
	printNode(endNode);
#endif // DEBUG

	Node curr = startNode;
	unsigned int ite = 0;
	while (!isStackEmpty(open) && ite < 50000)
	{
		ite++;

		// Getting the the lowest f_cost node in open
		Node cursor = curr;
		unsigned int lowest = INT_MAX;
		for (int i = 0; i < open->eltsCount; i++)
		{
			curr = open->array[i];
			if (curr.f_cost < lowest)
			{
				lowest = curr.f_cost;
				cursor = curr;
			}
		}

#ifdef DEBUG
		printf("\nBest node:");
		printNode(cursor);
#endif // DEBUG


		//pull(open, &bestNode);
		// Remove cursor from open to place it into closed
		pullNode(open, cursor);
		if (!isInStack(closed, cursor))
			push(closed, cursor);

		// If we found the path we break => we don't need to go further
		if (cursor.pos.x == endNode.pos.x && cursor.pos.y == endNode.pos.y)
			break;

		// Get Node neighbors
		Node neighbors[4];
		neighbors[0] = getNodeFromMap(map, (Vec2) { cursor.pos.x, cursor.pos.y - 1 });
		neighbors[1] = getNodeFromMap(map, (Vec2) { cursor.pos.x + 1, cursor.pos.y });
		neighbors[2] = getNodeFromMap(map, (Vec2) { cursor.pos.x, cursor.pos.y + 1 });
		neighbors[3] = getNodeFromMap(map, (Vec2) { cursor.pos.x - 1, cursor.pos.y });

#ifdef DEBUG
		printf("\nBest Node neighbors: ");
		for (size_t i = 0; i < 4; i++)
		{
			printNode(neighbors[i]);
		}
#endif // DEBUG

		// Getting the closest neighbor
		Node closest = cursor;
		for (int i = 0; i < 4; i++)
		{
			Vec2 pos = neighbors[i].pos;
			int cell = map->data[pos.x][pos.y];
			
			// Is the node traversable or already traversed ?
			if ((cell != 1 && cell != 0) || isInStack(closed, neighbors[i]))
				continue;

			// Geting the closest node or one that haven't been traversed
			if (neighbors[i].f_cost < closest.f_cost || !isInStack(open, neighbors[i]))
				closest = neighbors[i];
		}
		if (!isInStack(open, closest))
			push(open, closest), curr = closest;

#ifdef DEBUG
		printf("\nMoving to:");
		printNode(closest);
#endif // DEBUG
	}

	while (!isStackEmpty(closed))
	{
		Node tmp;
		pull(closed, &tmp);
		map->data[tmp.pos.x][tmp.pos.y] = -1;
	}
#ifdef DEBUG
	print_shard(map, &printPath);
#endif

	return SUCCESS;
}