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

	// Entree / Sortie
	map->entry.x = 1, map->entry.y = 1;

	Vec2 newPos = map->entry;
	Vec2 lastPos = newPos;
	// Vec2 temp;
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

		//si joueur descend + de la moitié du maze
		//
		if ((map->size.y-2 == newPos.y))
		{
			map->exit = newPos;
			// Vec2 temp = {-1,-1};
			// map->exit = temp;
			break;
		}
	}

	int ite = 0;
	// Plus c'est dur, plus on a de leurres
	switch (diff)
	{
	case DIFF_EASY:
		ite = floor(map->size.x * map->size.y / 10);
		break;
	case DIFF_MEDIUM:
		ite = floor(map->size.x * map->size.y / 7);
		break;
	case DIFF_HARD:
		ite = floor(map->size.x * map->size.y / 4);
		break;
	default:
		break;
	}

	// Génération d'obstacles leurres
	for (int i = 0; i < ite; i++)
	{
		// Coordonnées aléatoires
		Vec2 pos = { (rand() % (map->size.x - 1)) + 1, (rand() % (map->size.y - 1)) + 1 };

		int* cell = &map->data[pos.x][pos.y];
		if (*cell != 3 && *cell != 1 && *cell != 4)
			*cell = 4;
	}

	return map;
}

void printMapInfo(Map* map){
	printf("exit x %d, y %d",map->exit.x,map->exit.y);
	printf("\nWidth : %d, Height : %d, level : %d",map->size.x,map->size.y,map->level);
}

void printMapData(Map* map, int x, int y) {
	printf("%4d ", map->data[x][y]);
}

void printPath(Map* map,int x,int y) {
	char chr = 'a';
	if(map->entry.x == y && map->entry.y == x) chr = 'e';
	else if(map->exit.x == y && map->exit.y == x) chr = 's';
	else if(map->data[x][y] == 3) chr = 'W';
	else if (map->data[x][y] == 0) chr = ' ';
	else if (map->data[x][y] == 1) chr = '@';
	else if (map->data[x][y] == -1) chr = 'O';
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

	const char* footer = "\"start\":{\"x\":%2d,\"y\":%2d},\"end\":{\"x\":%2d,\"y\":%2d}";
	char* footer_buff = (char*)calloc(strlen(footer),sizeof(char));
	sprintf(footer_buff,footer,map->entry.x,map->entry.y,map->exit.x,map->exit.y);

	int _data_size = 6*map->size.x*map->size.y+1;
	
	char* data_buffer = (char*)calloc(_data_size,sizeof(char));
	int last_mazeblock_index = map->size.y * map->size.x;
	char* default_parsing_string = "%3s,";
	char* _default_parsing_string;


	for (int i = 0; i < map->size.y; i++)
	{
		for (int j = 0; j < map->size.x; j++)
		{
			char c_buffer[6];
			_default_parsing_string = default_parsing_string;

			if ((i+1)*(+1+j) == last_mazeblock_index) {
				_default_parsing_string = "%3s";
			}

			if(0 > sprintf_s(c_buffer,5, _default_parsing_string, renderPos(map->data[j][i]))){
				printf("\nerror in sprintf");
			}
			if(strcat_s(data_buffer,_data_size,c_buffer) != 0){
				printf("\nerror");
			}
		}
	}

	char* body = "\"body\":{\n\t\t%s,\"texture\":[%s],\"data\":[%s]}}";
	int body_size = strlen(body)+2*strlen(data_buffer)+strlen(footer_buff);
	char* body_buff = (char*)calloc(body_size,sizeof(char));

	if( 0 > sprintf_s(body_buff,body_size,body,footer_buff,data_buffer,data_buffer)){
		printf("error in sprintf_s");
	}
	free(data_buffer);
	fprintf(fichier_data,"%s",body_buff);
	free(body_buff);
	free(footer_buff);

	// fprintf(fichier_data,"%s",footer_buff);

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


int posToMap(int pos){
	int buffer;
	switch (pos)
	{
	case T_WALL:
		buffer = 0;
		break;
	case T_ICE:
		buffer = 1;
		break;
	case T_GRD:
		buffer = 3;
		break;
	case T_ROCK:
		buffer = 4;
		break;
	default:
		buffer = 9;
		break;
	}
	return buffer;
}

Map* parseJson(Map* map,char* filename){
	if (!filename) return NULL;

	FILE* fichier_data;
	errno_t err = fopen_s(&fichier_data, filename, "r+");

	if (err)
	{
		perror("fopen");
		return(NULL);
	}

	map = (Map*)calloc(1, sizeof(Map));

	if (!map)
	{
		free(map);
		return NULL;
	}

	char _width[4];
	char _height[4];
	char _diff[2];

	fseek(fichier_data,27,0);
	fread(_width,2,1,fichier_data);
	fseek(fichier_data,44,0);
	fread(_height,2,1,fichier_data);
	fseek(fichier_data,60,0);
	fread(_diff,1,1,fichier_data);


	char _entry_x[3];
	char _entry_y[3];
	fseek(fichier_data,94,0);
	fread(_entry_x,2,1,fichier_data);
	fseek(fichier_data,101,0);
	fread(_entry_y,2,1,fichier_data);

	int x = strtol(_entry_x,NULL,10);
	int y = strtol(_entry_y,NULL,10);
	Vec2 entry = {x,y};
	map->entry = entry;
	char _exit_x[3];
	char _exit_y[3];
	fseek(fichier_data,116,0);
	fread(_exit_x,2,1,fichier_data);
	fseek(fichier_data,123,0);
	fread(_exit_y,2,1,fichier_data);
	Vec2 exit = {strtol(_exit_x,NULL,10),strtol(_exit_y,NULL,10)};
	map->exit = exit;

	int width = strtol(_width,NULL,10);
	int height = strtol(_height,NULL,10);
	int diff = strtol(_diff,NULL,10);
	printf("entry x,y = %d,%d and exit x,y = %d,%d",entry.x,entry.y,exit.x,exit.y);
	// printf("Width = %d, Height = %d, diff = %d\n", width,height,diff);
	Vec2 size = {width,height};
	map->size = size;
	map->level = diff;


	map->data = (int**)calloc(size.x, sizeof(int*)); // Malloc des x
	for (int i = 0; i < size.x; i++)
		map->data[i] = (int*)calloc(size.y, sizeof(int)); // Malloc des y

	if (!map->data)
	{
		free(map->data);
		printf("error");
		return NULL;
	}
	
	int* textures = NULL;
	int* map_data = NULL;

	int res = width*height;
	// texture // data
	fseek(fichier_data,139,0);
	char tmp_val[4];
	int pos;
	for(int i = 0;i<height;i++){
		for(int j = 0;j<width;j++){
			fread(tmp_val,2,1,fichier_data);
			fseek(fichier_data,2,SEEK_CUR);
			// printf("%ld ",strtol(tmp_val,NULL,10));
			// printf("%d ",posToMap(strtol(tmp_val,NULL,10)));
			pos = posToMap(strtol(tmp_val,NULL,10));
			// map->data[i][j] =  strtol(tmp_val,NULL,10);
			map->data[i][j] = pos;
		}
	 	// printf("\n");
	}
	// entry :
	fclose(fichier_data);
	print_shard(map,&printMapData);
	printf("\nParsing successful.");
	return map;
} 