#include "mapgen.h"
#include "stack.h"
#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#define DEBUG

bool isInStack(Stack* stack, Vec2 vec)
{
	for (int i = 0; i < stack->eltsCount; i++)
	{
		if (stack->array[i].x == vec.x && stack->array[i].y == vec.y)
			return true;
	}
	return false;
}
//
//void pullNode(Stack* stack, Node node)
//{
//	int idx = 0;
//	bool found = false;
//	for (idx; idx < stack->eltsCount; idx++)
//	{
//		if (stack->array[idx].pos.x == node.pos.x && stack->array[idx].pos.y == node.pos.y)
//		{
//			found = true;
//			break;
//		}
//	}
//
//	if (!found) return;
//
//	for (int i = idx; i < stack->size - 1; i++)
//	{
//		stack->array[i] = stack->array[i + 1];
//	}
//	stack->eltsCount--;
//}

unsigned int ManDist(Vec2 a, Vec2 b)
{
	// * 10 pour avoir plus de précision (en gros 1 chiffre après la virgule mais sans virgule)
	return (unsigned int)(10 * (abs(b.x - a.x) + abs(b.y - a.y))); // Distance Manhattan
}

unsigned int EuclDist(Vec2 a, Vec2 b) // Heuristic
{
	if (a.x == b.x && a.y == b.y) return 0;
	return (unsigned int)(10 * sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2))); // Distance Euclidienne
}

bool isValid(Map* map, Vec2 pos)
{
	return (pos.x >= 0 && pos.x < map->size.x && pos.y >= 0 && pos.y < map->size.y);
}

bool isWalkable(Map* map, Vec2 pos)
{
	return (isValid(map, pos) && map->data[pos.x][pos.y] != T_WALL && map->data[pos.x][pos.y] != D_ROCK);
}

//Node getNodeFromMap(Map* map, Vec2 pos)
//{
//	if (!map || !isValid(map, pos)) return;
//
//	Node tmp;
//	tmp.pos = pos;
//	tmp.g_cost = ManDist(pos, map->entry);
//	tmp.h_cost = ManDist(pos, map->exit);
//	tmp.f_cost = tmp.g_cost + tmp.h_cost;
//	tmp.walkable = isWalkable(map, pos);
//	tmp.parent = NULL;
//
//	return tmp;
//}
//
//void printNode(Node node)
//{
//	printf("\n[+] Node:\nPos: (%d, %d)\nG_cost: %d\nH_cost: %d\nF_cost: %d\nWalkable: %d\n", node.pos.x, node.pos.y, node.g_cost, node.h_cost, node.f_cost, node.walkable);
//}

int countNeighbors(Map* map, Vec2 pos) // Compte les voisins aux 4 points cardinaux de pos
{
	if (!map) return ERROR;

	int nbNeighbors = 0;

	for (int y = pos.y - 1; y <= pos.y + 1; y++)
	{
		Vec2 _pos = { pos.x, y };
		if (isWalkable(map, pos) && y != pos.y)
			nbNeighbors++;
	}

	for (int x = pos.x - 1; x <= pos.x + 1; x++)
	{
		Vec2 _pos = { x, pos.y };
		if (isWalkable(map, _pos) && x != pos.x)
			nbNeighbors++;
	}

	return nbNeighbors;
}

bool canMove(Map* map, Vec2 pos, Dir dir)
{
	exec_move(&pos, dir);
	return isWalkable(map, pos);
}

// Algorithme A*
//int megaSolver2000(Map* map)
//{
//	if (!map) return ERROR;
//
//	// Stack of nodes to be evaluated
//	Stack* open;
//	NewStack(&open, map->size.x * map->size.y);
//
//	// Stack of nodes already evaluated
//	Stack* closed;
//	NewStack(&closed, map->size.x * map->size.y);
//
//	Stack* output;
//	NewStack(&output, map->size.x * map->size.y);
//
//	Node startNode = getNodeFromMap(map, map->entry);
//	Node endNode = getNodeFromMap(map, map->exit);
//
//	push(open, startNode);
//
//#ifdef DEBUG
//	printf("\nStart node:");
//	printNode(startNode);
//
//	printf("\nEnd node:");
//	printNode(endNode);
//#endif // DEBUG
//
//	Node curr = startNode;
//	unsigned int ite = 0;
//	while (!isStackEmpty(open) && ite < 50000)
//	{
//		ite++;
//		// Getting the the lowest f_cost node in open
//		//curr = open->array[0];
//		//for (int i = 0; i < open->eltsCount; i++)
//		//{
//		//	if (open->array[i].f_cost < curr.f_cost || open->array[i].f_cost == curr.f_cost && open->array[i].h_cost < curr.h_cost)
//		//		curr = open->array[i];
//		//}
//
//		int winner = 0;
//		for (int i = 0; i < open->eltsCount; i++)
//		{
//			if (open->array[i].f_cost < open->array[winner].f_cost)
//				winner = i;
//
//			if (open->array[i].f_cost == open->array[winner].f_cost)
//			{
//				//Prefer to explore options with longer known paths (closer to goal)
//				if (open->array[i].g_cost > open->array[winner].g_cost) {
//					winner = i;
//				}
//				if (open->array[i].g_cost == open->array[winner].g_cost && open->array[i].h_cost < open->array[winner].h_cost)
//				{
//					winner = i;
//				}
//		}
//	}
//
//		curr = open->array[winner];
//
//
//#ifdef DEBUG
//		printf("\nBest node:");
//		printNode(curr);
//#endif // DEBUG
//
//		// Remove curr from open to place it into closed
//		pullNode(open, curr);
//		if (!isInStack(closed, curr))
//			push(closed, curr);
//
//		// If we found the path we break => we don't need to go further
//		if (curr.pos.x == endNode.pos.x && curr.pos.y == endNode.pos.y)
//			break;
//
//		// Get Node neighbors
//		Node neighbors[4];
//
//		neighbors[0] = getNodeFromMap(map, (Vec2) { curr.pos.x, curr.pos.y - 1 });
//		neighbors[1] = getNodeFromMap(map, (Vec2) { curr.pos.x + 1, curr.pos.y });
//		neighbors[2] = getNodeFromMap(map, (Vec2) { curr.pos.x, curr.pos.y + 1 });
//		neighbors[3] = getNodeFromMap(map, (Vec2) { curr.pos.x - 1, curr.pos.y });
//
//		// Getting the closest neighbor
//		Node closest = curr;
//		for (int i = 0; i < 4; i++)
//		{
//			Vec2 pos = neighbors[i].pos;
//			int cell = map->data[pos.x][pos.y];
//			
//			// Is the node traversable or already traversed ?
//			if (!neighbors[i].walkable || isInStack(closed, neighbors[i]) || countNeighbors(map, pos) <= 1)
//				continue;
//
//			// Geting the closest node or one that haven't been traversed
//			unsigned int dist = curr.g_cost + ManDist(curr.pos, pos);
//			if (dist < neighbors[i].g_cost || !isInStack(open, neighbors[i]))
//			{
//				neighbors[i].g_cost = dist;
//				neighbors[i].h_cost = ManDist(curr.pos, pos);
//				neighbors[i].f_cost = neighbors[i].g_cost + neighbors[i].h_cost;
//				neighbors[i].parent = &curr;
//				curr = neighbors[i];
//
//				// Ici ça trouve à chaque fois mais impossible d'extraire le chemin à cause de parent
//				if (!isInStack(open, curr))
//					push(open, curr);
//
//			}
//		}
//		// Si on met le if ici, ça marche dans le cas où on trouve direct
//		//if (!isInStack(open, curr))
//		//	push(open, curr);
//
//#ifdef DEBUG
//		printf("\nMoving to best neighbor:");
//		printNode(closest);
//#endif // DEBUG
//	}
//
//	bool found = true;
//
//	while (!isStackEmpty(closed))
//	{
//		Node tmp;
//		pull(closed, &tmp);
//		map->data[tmp.pos.x][tmp.pos.y] = -1;
//	}
//
//	print_shard(map, &printPath);
//
//	return (found ? SUCCESS : ERROR);
//}

bool isEqual(Vec2 a, Vec2 b)
{
	return (a.x == b.x && a.y == b.y);
}

int megaSolver3000(Map* map)
{
	// Va contenir notre chemin
	Stack* path;
	int res = map->size.x * map->size.y;
	NewStack(&path, res);

	//Stack* blocked;
	//NewStack(&blocked, res);

	Vec2 pos = map->entry, oldPos = pos, tmp;
	push(path, pos);

	unsigned int ite = 0;

	while (!isEqual(pos, map->exit) && ite < 5000)
	{
		ite++;

		Dir dir = DIR_RIGHT;

		// Coords des 4 voisins cardinaux
		Vec2 neighbors[4] = {pos, pos, pos, pos};

		exec_move(&neighbors[0], DIR_UP);
		exec_move(&neighbors[1], DIR_RIGHT);
		exec_move(&neighbors[2], DIR_DOWN);
		exec_move(&neighbors[3], DIR_LEFT);

		// Find closest neighbor
		unsigned int dst = INT_MAX;
		for (int i = 0; i < 4; i++)
		{
			int newDst = ManDist(neighbors[i], map->exit);

			if (!isWalkable(map, neighbors[i])/* || isEqual(pos, neighbors[i])*/)
				continue;

			if (newDst < dst) // Le plus proche de l'arrivée
			{
				dst = newDst;
				if (i == 0) dir = DIR_UP;
				else if (i == 1) dir = DIR_RIGHT;
				else if (i == 2) dir = DIR_DOWN;
				else if (i == 3) dir = DIR_LEFT;
			}
		}
		
		// Tant qu'on a pas rencontré un obstacle, on continue de glisser
		while (canMove(map, pos, dir))
		{
			exec_move(&pos, dir);
			if (!isInStack(path, pos))
				push(path, pos);
			oldPos = pos;
		}

		//if (countNeighbors(map, pos) <= 2 && !isEqual(pos, map->exit))
		//{
		//	pull(path, &pos);
		//	push(blocked, pos);
		//	oldPos = pos;
		//}
	}

	// On crée le chemin
	while (!isStackEmpty(path))
	{
		Vec2 tmp;
		pull(path, &tmp);
		map->data[tmp.x][tmp.y] = -1;
	}
	print_shard(map, &printPath);
}