#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "mapgen.h"




bool checkPos(Map* map, Vec2 pos)
{
	return (pos.x >= 1 && pos.x < map->size.x - 1 && pos.y >= 1 && pos.y < map->size.y - 1);
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
		if ((_pos.x != lastPos.x || _pos.y != lastPos.y) && checkPos(map, _pos) && countNeighbors(map, _pos) == 1)
			*pos = _pos;
	}
}

Map* genMap(Map* map, Vec2 size, Difficulty diff)
{
	if (size.x < 5 || size.y < 5 || size.x > X_MAX || size.y > Y_MAX) return NULL;
	//if (size.x % 2 == 0) size.x--;
	//if (size.y % 2 == 0) size.y--;

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

	// Cr�ation des bordures
	for (int y = 0; y < map->size.x; y++)
	{
		for (int x = 0; x < map->size.x; x++)
		{
			if (x == 0 || y == 0 || x == map->size.x - 1 || y == map->size.y - 1)
				map->data[x][y] = 3;
		}
	}

	//print_shard(map, &printMapData);

	// Entree / Sortie
	map->entry.x = 1, map->entry.y = 1;

	Vec2 newPos = map->entry;
	Vec2 lastPos = newPos;
	Vec2 temp;
	Dir newDir = DIR_RIGHT, lastDir = newDir;

	for (int i = 0; i < map->size.x * map->size.y; i++)
	{
		int dir = rand() % 4;

		checkDir(map, &newPos, lastPos, dir);
		if (newPos.x != lastPos.x || newPos.y != lastPos.y)
			newDir = dir;

		if(lastDir != newDir) {
			addCorner(map,cornerPos(lastPos,lastDir));
		}
		lastPos = newPos;
		lastDir = newDir;
		map->data[newPos.x][newPos.y] = 1;

		if (newPos.x == map->size.x - 2 || newPos.y == map->size.y - 2)
		{
			//if (newPos.x == map->size.x - 2) 
			//	map->exit = (Vec2){ newPos.x + 1, newPos.y };
			//else
			//	map->exit = (Vec2){ newPos.x, newPos.y + 1 };
			map->exit = newPos;
			//map->data[map->exit.x][map->exit.y] = 1;
			break;
		}
	}


	return map;
}


void printMapData(Map* map, int x, int y) {
	printf("%4d ", map->data[x][y]);
}

void printPath(Map* map,int x,int y) {
	char chr = 'a';
	if (map->data[x][y] == 0) chr = ' ';
	else if (map->data[x][y] == 1) chr = '@';
	else if (map->data[x][y] == 5) chr = 'O';
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


char* renderPos(int posValue){
	// char buffer[4];
	char* buffer = (char*)calloc(4, sizeof(char));
	int buff_sz = 3;
	switch (posValue)
	{
	case 0:
		sprintf_s(buffer, buff_sz,"%d",T_WALL);
		break;
	case 1:
		sprintf_s(buffer, buff_sz,"%d",T_ICE);
		break;
	case 3:
		sprintf_s(buffer, buff_sz,"%d",T_GRD);
		break;
	case 4:
		sprintf_s(buffer, buff_sz,"%d",T_ROCK);
		break;
	default:
		break;
	}
	return buffer;

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

	const char* header = "{\n\t\"header\": {\n\t\"width\": %d,\n\t\"height\" : %d,\n\t \"diff\" : %d\n\t},\n\t";
	char* header_buff = (char*)calloc(strlen(header),sizeof(char));
	sprintf(header_buff,header,map->size.x,map->size.y,map->level);
	fprintf(fichier_data,"%s",header_buff);
	free(header_buff);

	const char* footer = "\"start\":{\"x\": %d ,\"y\": %d },\"end\":{\"x\":%d,\"y\":%d}}}";
	char* footer_buff = (char*)calloc(strlen(footer),sizeof(char));
	sprintf(footer_buff,footer,map->entry.x,map->entry.y,map->exit.x,map->exit.y);

	int _data_size = 5*map->size.x*map->size.y+1;
	
	char* data_buffer = (char*)calloc(_data_size,sizeof(char));
	int last_mazeblock_index = map->size.y * map->size.x;
	char* default_parsing_string = "%s, ";
	char* _default_parsing_string;


	for (int i = 0; i < map->size.y; i++)
	{
		for (int j = 0; j < map->size.x; j++)
		{
			char c_buffer[6];
			_default_parsing_string = default_parsing_string;

			if ((i+1)*(+1+j) == last_mazeblock_index) {
				_default_parsing_string = "%s";
			}

			if(0 > sprintf_s(c_buffer,5, _default_parsing_string, renderPos(map->data[j][i]))){
				printf("\nerror in sprintf");
			}
			if(strcat_s(data_buffer,_data_size,c_buffer) != 0){
				printf("\nerror");
			}
		}
	}

	char* body = "\"body\":{\n\t\t\"texture\":[%s],\"data\":[%s],";
	int body_size = strlen(body)+2*strlen(data_buffer);
	char* body_buff = (char*)calloc(body_size,sizeof(char));

	if( 0 > sprintf_s(body_buff,body_size,body,data_buffer,data_buffer)){
		printf("error in sprintf_s");
	}
	free(data_buffer);
	fprintf(fichier_data,"%s",body_buff);
	free(body_buff);

	fprintf(fichier_data,"%s",footer_buff);
	free(footer_buff);

	printf("Done Writing in %s and in x sec!\n",fichier);
	fclose(fichier_data);

	return SUCCESS;
}


Vec2 cornerPos(Vec2 pivot,Dir from){
	int x = 0,y=0 ;
	//printf("\n pivot : %d %d",pivot.x,pivot.y);
	if (from == DIR_UP) {
		x = pivot.x, y = pivot.y - 1;
	}
	else if(from == DIR_DOWN){
		x = pivot.x, y = pivot.y+1;
	}
	else if(from == DIR_LEFT){
		x = pivot.x-1, y = pivot.y;
	}
	else if(from == DIR_RIGHT){
		x = pivot.x+1, y = pivot.y;
	}
	//printf("\n x,y : %d %d",x,y);
	Vec2 corner = {x,y};
	return corner;
	
}

int addCorner(Map* map,Vec2 corner){
	if (checkPos(map, corner))
		map->data[corner.x][corner.y] = 4;
	return SUCCESS;
}