#pragma once
#include <stdbool.h>
#include <stdio.h>

#define ERROR -1
#define SUCCESS 0



#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

#define PATH 1
#define D_ROCK 7
//T_NAME <=> texture
#define T_ROCK 0
#define T_ICE -1
#define T_GRD -3
#define T_WALL -2

#define X_MAX 999
#define Y_MAX 999

#define RATE_EASY 0.05
#define RATE_MID 0.07
#define RATE_HARD 0.1



typedef enum DIFFICULTY
{
	DIFF_EASY = 1,
	DIFF_MEDIUM = 2,
	DIFF_HARD = 3,
}Difficulty;

typedef enum DIR
{
	DIR_UP = UP,
	DIR_DOWN = DOWN,
	DIR_LEFT = LEFT,
	DIR_RIGHT = RIGHT
}Dir;

typedef struct vec2 {
	int x, y;
}Vec2;

typedef struct map {
	Vec2 entry, exit, size;
	int** data;
	Difficulty level;
}Map;


int getValFromPos(Map* map, Vec2 pos);
int rangedRandWrapped(int range_min,int range_max);
int rangedRand(int range_min, int range_max);
bool check_pos(int _data);
int count_cardinals(Map* map,Vec2 pos);

void exec_move(Vec2* pos,Dir dir);
bool check_move(Map* map,Vec2 pos,Dir dir);

Map* init_memory(Map* map,Vec2 size,int diff);
Map* init_wall(Map* map);
Map* init_path(Map* map);
Map* init_fake(Map* map);
Map* genMap(Map* map, Vec2 size, Difficulty diff);

void printMapInfo(Map* map);
void printMapData(Map* map, int x, int y);
void printPath(Map* map,int x,int y);
void print_shard(Map* map,void (*fct)(Map*,int,int));


char* renderPos(int posValue);
FILE* open_file(FILE* file,char* filename,char* mode);


int export_map(Map* map, char* fichier);
Vec2 cornerPos(Vec2 pivot,Dir from);
int addCorner(Map* map,Vec2 pivot,Dir from);

int posToMap(int pos);
int string_to_int(FILE* file,int seek,int seek_from,int readsize,int count);
Map* import_map(Map* map,char* filename);

//unsigned int calcDist(Vec2 a, Vec2 b);
//Vec2* solver(Vec2* soluce, char* json);