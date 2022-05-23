#include "mapgen.h"
#include "stack.h"
#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#define DEBUG

bool isEqual(Vec2 a, Vec2 b)
{
	return (a.x == b.x && a.y == b.y);
}

bool isInStack(Stack* stack, Vec2 vec)
{
	for (int i = 0; i < stack->eltsCount; i++)
	{
		if (isEqual(stack->array[i], vec))
			return true;
	}
	return false;
}

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

int countNeighbors(Map* map, Vec2 pos, Stack* stack) // Compte les voisins aux 4 points cardinaux de pos
{
	if (!map) return ERROR;

	int nbNeighbors = 0;

	for (int y = pos.y - 1; y <= pos.y + 1; y++)
	{
		Vec2 _pos = { pos.x, y };
		if (isWalkable(map, pos) && y != pos.y && !isInStack(stack, pos))
			nbNeighbors++;
	}

	for (int x = pos.x - 1; x <= pos.x + 1; x++)
	{
		Vec2 _pos = { x, pos.y };
		if (isWalkable(map, _pos) && x != pos.x && !isInStack(stack, pos))
			nbNeighbors++;
	}

	return nbNeighbors;
}

bool canMove(Map* map, Vec2 pos, Dir dir)
{
	exec_move(&pos, dir);
	return isWalkable(map, pos);
}

int megaSolver3000(Map* map) {
	if (!map) return ERROR;

	// Va contenir notre chemin
	Stack* path;
	int res = map->size.x * map->size.y;
	NewStack(&path, res);

	Stack* blocked;
	NewStack(&blocked, res);

	Vec2 pos = map->entry, oldPos = pos;
	push(path, pos);

	unsigned int ite = 0;

	while (!isEqual(pos, map->exit) && ite < res) {
		ite++;

		Dir dir = DIR_DOWN;

		// Coords des 4 voisins cardinaux
		Vec2 neighbors[4] = {pos, pos, pos, pos};

		exec_move(&neighbors[0], DIR_UP);
		exec_move(&neighbors[1], DIR_RIGHT);
		exec_move(&neighbors[2], DIR_DOWN);
		exec_move(&neighbors[3], DIR_LEFT);

		// Find closest neighbor
		unsigned int dst = INT_MAX;
		for (int i = 0; i < 4; i++) {
			int newDst = ManDist(neighbors[i], map->exit);

			if (!isWalkable(map, neighbors[i]) || countNeighbors(map, neighbors[i], path) == 0 || isInStack(blocked, neighbors[i]))
				continue;

			if (newDst < dst) {
				dst = newDst;
				if (i == 0) dir = DIR_UP;
				else if (i == 1) dir = DIR_RIGHT;
				else if (i == 2) dir = DIR_DOWN;
				else if (i == 3) dir = DIR_LEFT;
			}
			else {
				dst = newDst;
				if (i == 0) dir = DIR_UP;
				else if (i == 1) dir = DIR_RIGHT;
				else if (i == 2) dir = DIR_DOWN;
				else if (i == 3) dir = DIR_LEFT;
			}
		}
		
		// Tant qu'on a pas rencontré un obstacle, on continue de glisser
		while (canMove(map, pos, dir) && !isEqual(pos, map->exit)) {
			exec_move(&pos, dir);

			if (!isInStack(path, pos))
				push(path, pos);
			oldPos = pos;
		}

		if (countNeighbors(map, pos, path) == 0 && !isEqual(pos, map->exit)) {
			if (!isInStack(blocked, pos))
				push(blocked, pos);
			//pull(path, &pos);
			oldPos = pos;
		}

		//if (countNeighbors(map, pos) <= 2 && !isEqual(pos, map->exit))
		//{
		//	pull(path, &pos);
		//	push(blocked, pos);
		//	oldPos = pos;
		//}
	}

	Vec2 tmp;
	peek(path, &tmp);
	if (isEqual(tmp, map->exit)) {
		printf("\n[+] Path found !\n");
	}
	else {
		printf("\n[+] Unable to find a path.\n");
	}

	// On crée le chemin
	while (!isStackEmpty(path)) {
		pull(path, &tmp);
		map->data[tmp.x][tmp.y] = -1;
	}
	print_shard(map, &printPath);

	return SUCCESS;
}