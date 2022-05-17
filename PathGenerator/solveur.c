#include "mapgen.h"
#include <math.h>
#include <stdio.h>

bool canMove(Map* map, Vec2 pos, Dir dir)
{
	return (checkPos(map, pos) && map->data[pos.x][pos.y] != 3 && map->data[pos.x][pos.y] != 4);
}

void move(Map* map, Vec2* pos, Dir dir)
{
	Vec2 _pos = *pos;
	switch (dir)
	{
	case DIR_UP:
		_pos.y--;
		break;
	case DIR_DOWN:
		_pos.y++;
		break;
	case DIR_LEFT:
		_pos.x--;
		break;
	case DIR_RIGHT:
		_pos.x++;
		break;
	default:
		break;
	}
	if (canMove(map, _pos, dir))
		*pos = _pos;
}

unsigned int calcDist(Vec2 a, Vec2 b)
{
	if (a.x == b.x && a.y == b.y) return 0;
	// * 100 pour avoir plus de pr�cision (en gros 2 chiffre apr�s la virgule mais sans virgule)
	return (unsigned int)(100 * sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2)));
}

/*bool opVec2(Vec2 a, Vec2 b, char operator)
{
	switch (operator)
	{
	case '=':
		return (a.x == b.x && a.y == b.y);
		break;
	case '<=':
		return (a.x <= b.x || a.y <= b.y);
		break;
	case '>=':
		return (a.x >= b.x || a.y >= b.y);
		break;
	case '<':
		return (a.x < b.x || a.y < b.y);
		break;
	case '>':
		return (a.x > b.x || a.y > b.y);
		break;
	default:
		return false;
		break;
	}
}*/

int getNeighbor(Map* map, Vec2 pos, Dir dir)
{
	switch (dir)
	{
	case DIR_UP:
		pos.y--;
		break;
	case DIR_DOWN:
		pos.y++;
		break;
	case DIR_LEFT:
		pos.x--;
		break;
	case DIR_RIGHT:
		pos.x++;
		break;
	default:
		break;
	}

	if (checkPos(map, pos, dir))
		return (map->data[pos.x][pos.y]);
	else return -1;
}

Dir getNewDir(Map* map, Vec2 pos, Dir dir)
{
	int neighbors[4]; // Liste des voisins
	Dir newDir[3] = { DIR_UNDEFINED, DIR_UNDEFINED, DIR_UNDEFINED }; // 3 directions possibles (on ne retourne pas sur nos pas)
	int ctr = 0;

	// On prend les 4 voisins
	neighbors[DIR_UP] = getNeighbor(map, pos, DIR_UP);
	neighbors[DIR_DOWN] = getNeighbor(map, pos, DIR_DOWN);
	neighbors[DIR_LEFT] = getNeighbor(map, pos, DIR_LEFT);
	neighbors[DIR_RIGHT] = getNeighbor(map, pos, DIR_RIGHT);

	for (int i = 0; i < 4; i++)
	{
		// Si le voisin n'est pas un obstacle et que ce n'est pas notre ancienne direction
		if (neighbors[i] != 4 && neighbors[i] != 3 && neighbors[i] != -1 && i != dir)
		{
			// On ajoute la nouvelle direction potentielle
			newDir[ctr] = i;
			ctr++;
		}
	}

	Vec2 closestObs;
	for (int i = 0; i < 3; i++)
	{
		if (newDir[i] == DIR_UP)
		{
			for (int y = pos.y; y >= 1; y--)
			{
				if (map->data[pos.x][y] == 4)
					closestObs = (Vec2){ pos.x, y };
			}
		}
		if (newDir[i] == DIR_DOWN)
		{
			for (int y = pos.y; y < map->size.y - 1; y++)
			{
				if (map->data[pos.x][y] == 4)
					closestObs = (Vec2){ pos.x, y };
			}
		}
		if (newDir[i] == DIR_LEFT)
		{
			for (int x = pos.x; x >= 1; x--)
			{
				if (map->data[x][pos.y] == 4)
					closestObs = (Vec2){ x, pos.y };
			}
		}
		if (newDir[i] == DIR_RIGHT)
		{
			for (int x = pos.x; x < map->size.x - 1; x++)
			{
				if (map->data[x][pos.y] == 4)
					closestObs = (Vec2){ x, pos.y };
			}
		}
	}

	return newDir[0];
}

int solver(Map* map)
{
	Vec2 pos = map->entry, lastPos = pos;
	Dir dir = DIR_RIGHT, lastDir = dir;
	unsigned int dst = calcDist(pos, map->exit);

	//for (int i = 1; i < map->size.y - 1; i++)
	//{
	//	for (int j = 1; j < map->size.x - 1; j++)
	//	{
	//		if (map->data[i][j] != 4)
	//			map->data[i][j] = calcDist((Vec2) { i, j }, map->exit);
	//	}
	//}

	pos = map->entry;

	for (size_t i = 0; i < 10; i++)
	{
		move(map, &pos, dir);
		map->data[pos.x][pos.y] = -1;

		if (getNeighbor(map, pos, dir) == 4)
		{
			dir = getNewDir(map, pos, dir);
		}
	}

	//while (getNeighbor(map, pos, DIR_RIGHT) != 4) // De base on va � droite
	//{
	//	move(map, &pos, DIR_RIGHT);
	//	map->data[pos.x][pos.y] = -1;
	//}

	//while (getNeighbor(map, pos, DIR_DOWN) != 4) // Puis on descend
	//{
	//	move(map, &pos, DIR_DOWN);
	//	map->data[pos.x][pos.y] = -1;
	//}

	//for (int i = 1; i < map->size.x - 1; i++) // On cherche l'obstacle le plus proche
	//{
	//	if (map->data[i][pos.y] == 4)
	//		dir = (i < pos.x) ? DIR_LEFT : DIR_RIGHT;
	//}

	//for (size_t i = 0; i < 100; i++)
	////while (dst != 0)
	//{
	//	while (lastDir == dir)
	//	{
	//		move(map, &pos, dir, dst);

	//		if (pos.x != lastPos.x || pos.y != lastPos.y)
	//		{
	//			map->data[pos.x][pos.y] = 5;
	//			dst = calcDist(pos, map->exit);
	//			lastPos = pos;
	//		}
	//		else
	//		{
	//			if (map->data[pos.x + 1][pos.y] == 3)
	//				dir = DIR_DOWN;
	//			else if (map->data[pos.x][pos.y + 1] == 3)
	//				dir = DIR_RIGHT;
	//		}
	//	}

	//	lastDir = dir;
	//}

	print_shard(map, &printMapData);

	return SUCCESS;
}