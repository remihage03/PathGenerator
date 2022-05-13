#pragma once
#include <stdbool.h>

#define ERROR -1
#define SUCCESS 0

#define T_ROCK 0
#define T_ICE -1
#define T_GRD -3
#define T_WALL -2

#define X_MAX 999
#define Y_MAX 999




typedef enum DIFFICULTY
{
	DIFF_EASY = 1,
	DIFF_MEDIUM = 2,
	DIFF_HARD = 3,
}Difficulty;

typedef enum DIR
{
	DIR_DOWN,
	DIR_LEFT,
	DIR_RIGHT
}Dir;

typedef struct vec2 {
	int x, y;
}Vec2;

typedef struct map {
	Vec2 entry, exit, size;
	int** data;
	Difficulty level;
}Map;

Map* genMap(Map* map, Vec2 size, Difficulty diff);

void printMapData(Map* map, int x, int y);
void printPath(Map* map,int x,int y);
void print_shard(Map* map,void (*fct)(Map*,int,int));


char* renderPos(int posValue);
int exportMap(Map* map, char* fichier);
bool checkPos(Map* map, Vec2 pos);
Vec2 cornerPos(Vec2 pivot,Dir from);
int addCorner(Map* map,Vec2 corner);

int solver(Map* map);
