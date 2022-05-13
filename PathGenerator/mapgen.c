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
		if ((_pos.x != lastPos.x || _pos.y != lastPos.y) && checkPos(map, _pos) && countNeighbors(map, _pos) == 1)
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
	Dir newDir = DIR_RIGHT, lastDir = newDir;

	for (int i = 0; i < map->size.x * map->size.y; i++)//while (newPos.x != map->size.x - 1 || newPos.y != map->size.y - 1) // Tant qu'on a pas atteint un bord
	{
		int dir = rand() % 3;

		checkDir(map, &newPos, lastPos, dir);
		if (newPos.x != lastPos.x || newPos.y != lastPos.y)
			newDir = dir;

		if(lastDir != newDir) {
			addCorner(map,cornerPos(lastPos,lastDir));
		}
		lastPos = newPos;
		lastDir = newDir;
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
	char chr = 'a';
	if (map->data[x][y] == 0) chr = ' ';
	else if (map->data[x][y] == 1) chr = '@';
	else chr = 'X';

	printf("%c ", chr);
}

void print_shard(Map* map,void (*fct)(Map*,int,int)) {
	printf("\n");
	for (int y = 0; y < map->size.y; y++)
	{
		for (int x = 0; x < map->size.x; x++)
		{
			(*fct)(map,x,y);
		}
		printf("\n");
	}
}

int exportMap(Map* map, char* fichier)
{
	if (!map || !fichier) return ERROR;

	FILE* fichier_data;
	errno_t err = fopen_s(&fichier_data, fichier, "w+");

	if (err)
	{
		perror("fopen");
		return(ERROR);
	}

	const char* json = "{\"options\":{\"width\": %d, \"height\" : %d, \"lvl\" : %d},\"data\":[";
	int newsize = 4 + 4 + strlen(json); //4 = strlen('1000') <=> strlen(maxsizeX/Y) 
	char* buffer = (char*)calloc(newsize, sizeof(char));

	sprintf_s(buffer, newsize, json, map->size.x, map->size.y, map->level);
	fprintf(fichier_data, "%s", buffer);

	int last_mazeblock_index = map->size.y * map->size.x;
	int check_last_block = 1;
	char* default_parsing_string = "%d,";
	char* _default_parsing_string;

	for (int i = 0; i < map->size.y; i++)
	{
		for (int j = 0; j < map->size.x; j++)
		{
			char c_buffer[5];
			_default_parsing_string = default_parsing_string;

			if (check_last_block == last_mazeblock_index) {
				_default_parsing_string = "%d";
			}

			sprintf_s(c_buffer, 4, _default_parsing_string, map->data[j][i]);
			fprintf(fichier_data, "%s", c_buffer);
			check_last_block++;
		}

		fprintf(fichier_data, "%s", "\n");
	}

	char* nextOpt = "],\"entry\":{\"x\":%d,\"y\":%d},\"exit\":{\"x\":%d,\"y\":%d}}";
	Vec2 in = map->entry, out = map->exit;
	
	sprintf_s(buffer, strlen(nextOpt), nextOpt, in.x, in.y, out.x, out.y);
	fprintf(fichier_data, "%s", buffer);
	
	printf("Done Writing in %s !\n", fichier);
	fclose(fichier_data);
	return SUCCESS;
}


Vec2 cornerPos(Vec2 pivot,Dir from){
	int x = 0,y=0 ;
	printf("\n pivot : %d %d",pivot.x,pivot.y);
	if(from == DIR_DOWN){
		x = pivot.x, y = pivot.y+1;
	}
	else if(from == DIR_LEFT){
		x = pivot.x-1, y = pivot.y;
	}
	else if(from == DIR_RIGHT){
		x = pivot.x+1, y = pivot.y;
	}
	printf("\n x,y : %d %d",x,y);
	Vec2 corner = {x,y};
	return corner;
	
}

int addCorner(Map* map,Vec2 corner){
	if (checkPos(map, corner))
		map->data[corner.x][corner.y] = 3;
	return SUCCESS;
}