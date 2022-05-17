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

	if (ctr == 1) return newDir[0];

	Vec2 obs[3] = {{-1, -1}, {-1, -1}, {-1, -1}};
	unsigned int oldDst;

	ctr = 0;

	// On enregistre les coordonn�es des obstacles les plus proches des direction possibles
	for (int i = 0; i < 3; i++)
	{
		if (newDir[i] == DIR_UP)
		{
			for (int y = pos.y; y >= 1; y--)
			{
				//if (map->data[pos.x][y] == -1) break;

				if (map->data[pos.x][y] == 4 || map->data[pos.x][y] == 3)
				{
					obs[ctr] = (Vec2){ pos.x, y };
					ctr++;
					break;
				}
			}
		}
		else if (newDir[i] == DIR_DOWN)
		{
			for (int y = pos.y; y < map->size.y - 1; y++)
			{
				//if (map->data[pos.x][y] == -1) break;

				if (map->data[pos.x][y] == 4 || map->data[pos.x][y] == 3)
				{
					obs[ctr] = (Vec2){ pos.x, y };
					ctr++;
					break;
				}
			}
		}
		else if (newDir[i] == DIR_LEFT)
		{
			for (int x = pos.x; x >= 1; x--)
			{
				//if (map->data[x][pos.y] == -1) break;
				if (map->data[x][pos.y] == 4 || map->data[x][pos.y] == 3)
				{
					obs[ctr] = (Vec2){ x, pos.y };
					ctr++;
					break;
				}
			}
		}
		else if (newDir[i] == DIR_RIGHT)
		{
			for (int x = pos.x; x < map->size.x - 1; x++)
			{
				//if (map->data[x][pos.y] == -1) break;
				if (map->data[x][pos.y] == 4 || map->data[x][pos.y] == 3)
				{
					obs[ctr] = (Vec2){ x, pos.y };
					ctr++;
					break;
				}
					
			}
		}
	}

	unsigned int closest = _CRT_INT_MAX;
	Dir ret = DIR_UNDEFINED;
	// On cherche l'obstacle le plus proche
	for (int i = 0; i < 3; i++)
	{
		if (obs[i].x != -1 && obs[i].y != -1)
		{
			if (calcDist(obs[i], pos) <= closest)
			{
				closest = calcDist(obs[i], pos);

				// On en d�duit la direction
				if (obs[i].x < pos.x) ret = DIR_LEFT;
				else if (obs[i].x > pos.x) ret = DIR_RIGHT;
				else if (obs[i].y < pos.y) ret = DIR_UP;
				else if (obs[i].y > pos.y) ret = DIR_DOWN;
			}
		}	
	}

	return ret;
}

int solver(Map* map)
{
	Vec2 pos = map->entry;
	Dir dir = DIR_RIGHT;

	//for (size_t i = 0; i < 100; i++)
	//{
	//	move(map, &pos, dir);
	//	map->data[pos.x][pos.y] = -1;

	//	if (getNeighbor(map, pos, dir) == 4 || getNeighbor(map, pos, dir) == 3)
	//	{
	//		dir = getNewDir(map, pos, dir);
	//	}
	//}

	for (int y = 1; y < map->size.y - 1; y++)
	{
		for (int x = 1; x < map->size.x - 1; x++)
		{
			if (map->data[x][y] != 4)
				map->data[x][y] = calcDist((Vec2) { x, y }, map->exit);
		}
	}


	//while (pos.x != map->exit.x && pos.y != map->exit.y)
	for (int i = 0; i < map->size.x * map->size.y * 10; i++)
	{
		int neighbors[4];
		//*neighbors = getNeighbors(map, pos);
		neighbors[DIR_UP] = getNeighbor(map, pos, DIR_UP);
		neighbors[DIR_DOWN] = getNeighbor(map, pos, DIR_DOWN);
		neighbors[DIR_LEFT] = getNeighbor(map, pos, DIR_LEFT);
		neighbors[DIR_RIGHT] = getNeighbor(map, pos, DIR_RIGHT);

		int smallest = 9999;
		Dir newDir = DIR_UNDEFINED;
		for (int i = 0; i < 4; i++)
		{
			if (neighbors[i] < smallest && neighbors[i] != -1 && neighbors[i] != -3 && neighbors[i] != 4)
			{
				smallest = neighbors[i];
				newDir = i;
			}
		}

		move(map, &pos, newDir);
		map->data[pos.x][pos.y] = -1;
		//print_shard(map, &printPath);

		if (pos.x == map->exit.x && pos.y == map->exit.y)
			break;
	}

	for (int y = 1; y < map->size.y - 1; y++)
	{
		for (int x = 1; x < map->size.x - 1; x++)
		{
			if (map->data[x][y] != 4 && map->data[x][y] != -1)
				map->data[x][y] = 0;
		}
	}

	return SUCCESS;
}