#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mapgen.h"

bool checkPos(Map* map, Vec2 pos)
{
	return (pos.x >= 0 && pos.x < map->size.x && pos.y >= 0 && pos.y < map->size.y);
}

int countNeighbors(Map* map, Vec2 pos) // Compte les voisins aux 4 points cardinaux de pos
{
	if (!map) return ERROR;

	int nbNeighbors = 0;

	for (int y = pos.y - 1; y <= pos.y +1 ; y++)
	{
		Vec2 _pos = { pos.x, y };
		if (checkPos(map, _pos))
			if (y != pos.y && map->data[pos.x][y] == 1)
				nbNeighbors++;
	}

	for (int x = pos.x - 1; x <= pos.x + 1; x++)
	{
		Vec2 _pos = { x, pos.y };
		if (checkPos(map, _pos))
			if (x != pos.x && map->data[x][pos.y] == 1)
				nbNeighbors++;
	}

	return nbNeighbors;
}

void checkDir(Map* map, Vec2* pos, Vec2 lastPos, Dir dir)
{
	if (rand() % 2 == 0)
	{
		Vec2 _pos = *pos;
		switch (dir)
		{
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
		if ((_pos.x != lastPos.x || _pos.y != lastPos.y) && checkPos(map, _pos) && countNeighbors(map, *pos) == 1)
			*pos = _pos;
	}
}

Map* genMap(Map* map, Vec2 size, Difficulty diff)
{
	if (size.x < 5 || size.y < 5 || size.x > X_MAX || size.y > Y_MAX) return NULL;
	if (size.x % 2 == 0) size.x--;
	if (size.y % 2 == 0) size.y--;

	map = (Map*)calloc(1, sizeof(Map));

	if (!map)
	{
		free(map);
		return NULL;
	}

	map->size = size;
	map->level = diff;

	map->data = (int**)calloc(size.x, sizeof(int*)); // Malloc des x
	for (int i = 0; i < size.x; i++)
		map->data[i] = (int*)calloc(size.y, sizeof(int)); // Malloc des y

	if (!map->data)
	{
		free(map->data);
		return NULL;
	}

	// Entree / Sortie
	map->entry.x = 0, map->entry.y = 1;
	map->exit.x = map->size.x - 1, map->exit.y = map->size.y - 2;

	Vec2 newPos = map->entry;
	Vec2 lastPos = newPos;
	Vec2 temp;
	Dir currDir = DIR_RIGHT, lastDir = currDir;

	for (int i = 0; i < map->size.x * map->size.y; i++)//while (newPos.x != map->size.x - 1 || newPos.y != map->size.y - 1) // Tant qu'on a pas atteint un bord
	{
		int dir = rand() % 3;

		switch (dir)
		{
		case DIR_DOWN:
		{
			//checkDir(map, &newPos, lastPos, dir);
			//currDir = DIR_DOWN;
			if (rand() % 2 == 0)
			{
				temp = newPos;
				temp.y++;
				if (temp.y != lastPos.y && checkPos(map, temp)/*temp.y < map->size.y*/ && countNeighbors(map, temp) == 1)
					newPos.y++, currDir = DIR_DOWN;
			}
			break;
		}
		case DIR_LEFT:
		{
			//checkDir(map, &newPos, lastPos, dir);
			//currDir = DIR_LEFT;
			if (rand() % 2 == 0)
			{
				temp = newPos;
				temp.x--;
				if (temp.x != lastPos.x && /*temp.x >= 0*/ checkPos(map, temp) && countNeighbors(map, temp) == 1)
					newPos.x--, currDir = DIR_LEFT;
			}
			break;
		}
		case DIR_RIGHT:
		{
			//checkDir(map, &newPos, lastPos, dir);
			//currDir = DIR_RIGHT;
			if (rand() % 2 == 0)
			{
				temp = newPos;
				temp.x++;
				if (temp.x != lastPos.x && checkPos(map, temp)/*temp.x < map->size.x*/ && countNeighbors(map, temp) == 1)
					newPos.x++, currDir = DIR_RIGHT;
			}
			break;
		}
		default:
			break;
		}

		lastPos = newPos;
		lastDir = currDir;
		map->data[newPos.x][newPos.y] = 1;

		if (newPos.x == map->size.x - 1 || newPos.y == map->size.y - 1)
		{
			map->exit = newPos;
			break;
		}
	}

	return map;
}


void printMapData(Map* map, int x, int y) {
	printf("%2d ", map->data[x][y]);
}

void printPath(Map* map,int x,int y) {
	printf("%c ", map->data[x][y] == 0 ? ' ' : '@');
}

void print_shard(Map* map,void (*fct)(Map*,int,int)) {
	for (int y = 0; y < map->size.y; y++)
	{
		for (int x = 0; x < map->size.x; x++)
		{
			(*fct)(map,x,y);
		}
		printf("\n");
	}
}

int exportMap(Map* map, char* fichier);



Vec2 cornerPos(Vec2 entry,Dir from){
	Vec2 corner;
	if(from == DIR_DOWN){
		corner = {entry.x,entry.y+1};
	}
	else if(from == DIR_LEFT){
		corner = {entry.x-1,entry.y};		
	}
	else if(from == DIR_RIGHT){
		corner = {entry.x+1,entry.y};
	}
	return corner;
}

int addCorner(Map* map,Vec2 corner){
	map->data[corner.x][corner.y] == "X";
	return SUCCESS;
}