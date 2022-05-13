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
	Dir currDir;

	for (int i = 0; i < map->size.x * map->size.y; i++)//while (newPos.x != map->size.x - 1 || newPos.y != map->size.y - 1) // Tant qu'on a pas atteint un bord
	{
		int dir = rand() % 3;

		switch (dir)
		{
		case DIR_DOWN:
		{
			if (rand() % 2 == 0)
			{
				temp = newPos;
				temp.y++;
				if (temp.y != lastPos.y && temp.y < map->size.y && countNeighbors(map, temp) == 1)
					newPos.y++, currDir = DIR_DOWN;
			}
			break;
		}
		case DIR_LEFT:
		{
			if (rand() % 2 == 0)
			{
				temp = newPos;
				temp.x--;
				if (temp.x != lastPos.x && temp.x >= 0 && countNeighbors(map, temp) == 1)
					newPos.x--, currDir = DIR_LEFT;
			}
			break;
		}
		case DIR_RIGHT:
		{
			if (rand() % 2 == 0)
			{
				temp = newPos;
				temp.x++;
				if (temp.x != lastPos.x && temp.x < map->size.x && countNeighbors(map, temp) == 1)
					newPos.x++, currDir = DIR_RIGHT;
			}
			break;
		}
		default:
			break;
		}

		lastPos = newPos;
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



int cornerPos(Vec2 entry,Vec2 exit,Dir entry_dir,Dir exit_dir){
	int dx = exit.x - entry.x;
	int dy = exit.y - entry.y;
	if (!dx){
		if(dy>0){
			return DIR_DOWN;
		}
		return DIR_UP;
	}
	else if(!dy){
		if(dx>0){
			return DIR
		}

	}

}

int addCorner(Map* map,Vec2 entry_pos,Vec2 exit_pos){
	
}