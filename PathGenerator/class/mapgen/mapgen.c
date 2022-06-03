#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "mapgen.h"
#include "../Vec2/Vec2.h"


// #include "class/Vec2/Vec2.h"


int getValFromPos(Map* map, Vec2 pos)
{
	if (!isValid(map, pos)) return -1;

	return map->data[pos.x][pos.y];
}

int rangedRandWrapped(int range_min,int range_max)
{
    int u = 0;
    while (u == 0 || u == 1)
    {
        u = rangedRand(range_min,range_max);
		// if(u %2 == 0)
			// return rangedRandWrapped(range_min,range_max);
    }
    return(u);
}
int rangedRand(int range_min, int range_max)
{
    int u = (int)((double)rand() / ((double)RAND_MAX + 1) * ((double)range_max - (double)range_min)) + range_min;
    return(u);
}

bool check_pos(int _data){
	return (!(_data == T_WALL || _data == PATH || _data == D_ROCK));
}

int count_cardinals(Map* map,Vec2 pos){
    int count = 0;
    if(isValid(map, (Vec2){pos.x, pos.y-1}) && !check_pos(map->data[pos.x][pos.y - 1])) count++; //up
    if(isValid(map, (Vec2) { pos.x+1, pos.y}) && !check_pos(map->data[pos.x+1][pos.y])) count++; //right
    if(isValid(map, (Vec2) { pos.x, pos.y+1 }) && !check_pos(map->data[pos.x][pos.y+1])) count++; //down
    if(isValid(map, (Vec2) { pos.x-1, pos.y }) && !check_pos(map->data[pos.x-1][pos.y])) count++; //left
    return count;
}

//déplace dans la direction voulue
void exec_move(Vec2* pos,Dir dir){
    if(dir == DIR_UP) pos->y--;
    else if(dir == DIR_RIGHT) pos->x++;
    else if(dir == DIR_DOWN) pos->y++;
    else if(dir == DIR_LEFT) pos->x--;
}

bool check_move(Map* map,Vec2 pos,Dir dir){
    Vec2 _pos = {pos.x,pos.y};
    exec_move(&_pos,dir);
    if (!isValid(map, _pos)) return false;
    if(!check_pos(map->data[_pos.x][_pos.y]) || count_cardinals(map, _pos) == 4) return false;
    return true;
}

bool isValid(Map* map, Vec2 pos)
{
	return (pos.x >= 0 && pos.x < map->size.x && pos.y >= 0 && pos.y < map->size.y);
}

Map* init_memory(Map* map,Vec2 size,int diff){
    map = (Map*)calloc(1, sizeof(Map));
	if (!map)
	{
		free(map);
		return NULL;
	}
    map->size = size;
	map->level = diff;
    Vec2 entry = {1,1};
    map->entry = entry;
	map->data = (int**)calloc(size.x, sizeof(int*)); // Malloc des x
    if (!map->data)
	{
		free(map->data);
		return NULL;
	}

    for (int i = 0; i < size.x; i++){
		map->data[i] = (int*)calloc(size.y, sizeof(int)); // Malloc des y
        if(!map->data[i]){
            free(map->data[i]);
            free(map->data);
            return NULL;
        }
    }
    return map;
}
Map* init_wall(Map* map){ // Inutile murs générés sur le jeu
    if(!map) return NULL;
    if(!map->data) return NULL;
    for (int y = 0; y < map->size.x; y++)
	{
		for (int x = 0; x < map->size.x; x++)
		{
			if (x == 0 || y == 0 || x == map->size.x - 1 || y == map->size.y - 1)
				map->data[x][y] = T_WALL;
		}
	}
    return map;
}

Map* init_path(Map* map){
    if(!(map && map->data)) return NULL;
    Vec2 current_pos = map->entry;
    Vec2 last_pos = map->entry;
	Dir newDir = DIR_RIGHT;
    Dir lastDir = DIR_RIGHT;
    int resolution = map->size.x * map->size.y;

	for (int i = 0; i < resolution; i++)
	{
        if(rand()%10 == 0)
            newDir = lastDir;
        else{
            newDir = rangedRandWrapped(-2,3);
        }
        if(check_move(map,current_pos,newDir)){
            exec_move(&current_pos,newDir);
            if(newDir != lastDir)
                if(addCorner(map,last_pos,lastDir) != ERROR)
                    // printf("\ncan't add corner at pos ..");
					map->tournant++;
            map->data[current_pos.x][current_pos.y] = PATH;
            lastDir = newDir;
        }
        last_pos = current_pos;

        if(current_pos.y == map->size.y-2) {
            Vec2 exit = {current_pos.x,current_pos.y};
            map->exit = exit;
            break;
        }
	}
    return map;
}

Map* init_fake(Map* map){
    if(!map || !map->data) return NULL;
    // print_shard(map,&printPath);
    float rate;
    if(map->level == DIFF_EASY) rate = RATE_EASY;
    else if(map->level == DIFF_MEDIUM) rate = RATE_MID;
    else if(map->level == DIFF_HARD) rate = RATE_HARD;
    int nbObs = floor(map->size.x*map->size.y* rate);
    while(nbObs>0)
    {
        int _x = rangedRand(2,map->size.x-1);
        int _y = rangedRand(2,map->size.y-1);
        if(check_pos(map->data[_x][_y])){
            map->data[_x][_y] = D_ROCK;
            nbObs--;
        }
    }
    // print_shard(map,&printPath);
    return map;
}

void printMapInfo(Map* map) {
	printf("exit x %d, y %d", map->exit.x, map->exit.y);
	printf("\nWidth : %d, Height : %d, level : %d", map->size.x, map->size.y, map->level);
}

void printMapData(Map* map, int x, int y) {
	printf("%2d ", map->data[x][y]);
}

void printPath(Map* map, int x, int y) {
	char chr = 'a';
	if (map->entry.x == y && map->entry.y == x) chr = 'E';
	else if (map->exit.x == x && map->exit.y == y) chr = 'S';
	else if (map->data[x][y] == T_WALL) chr = 'W';
	else if (map->data[x][y] == 1) chr = '@';
	else if (map->data[x][y] == -1) chr = '*';
	else if (map->data[x][y] == D_ROCK) chr = 'X';
	else chr = ' ';
	printf("%c ", chr);
}

void print_shard(Map* map, void (*fct)(Map*, int, int)) {
	printf("\n");
	for (int y = 0; y < map->size.y; y++)
	{
		for (int x = 0; x < map->size.x; x++)
		{
			(*fct)(map, x, y);
		}
		printf("\n");
	}
}

//only alloc memory then call other functinos
Map* genMap(Map* map, Vec2 size, Difficulty diff)
{
    //check si les tailles sont conformes
	if (size.x < 5 || size.y < 5 || size.x > X_MAX || size.y > Y_MAX) return NULL;
    if(!map) free(map);

    map = init_memory(map,size,diff);
    //map = init_wall(map); // Inutile murs générés sur le jeu
    map = init_path(map);
    map = init_fake(map);
	return map;
}

Vec2 cornerPos(Vec2 pivot, Dir from) {
	int x = pivot.x;
	int y = pivot.y;
	if (from == DIR_UP) y--;
	else if (from == DIR_RIGHT) x++;
	else if (from == DIR_DOWN)y++;
	else if (from == DIR_LEFT) x--;
	Vec2 corner = { x,y };
	return corner;
}

int addCorner(Map* map, Vec2 pivot, Dir from) {
	Vec2 corner = cornerPos(pivot, from);
    if (!isValid(map, corner)) return ERROR;
	int c = map->data[corner.x][corner.y];
	if (c != T_WALL && c != PATH) {
		map->data[corner.x][corner.y] = D_ROCK;
		return SUCCESS;
	}
	return ERROR;
}

// Import export
char* renderPos(int posValue){
	char* buffer = (char*)calloc(4, sizeof(char));
    if (!buffer) {
        free(buffer);
        return;
    }

	int buff_sz = 3;
	switch (posValue)
	{
	case T_WALL:
		sprintf_s(buffer, buff_sz,"%d",T_WALL);
		break;
	case PATH:
		sprintf_s(buffer, buff_sz,"%d",T_ICE);
		break;
	case 0:
		sprintf_s(buffer, buff_sz,"%d",T_ICE);
		break;
	case D_ROCK:
		sprintf_s(buffer, buff_sz,"%d",0);
		break;
	default:
		break;
	}
	return buffer;
}

FILE* open_file(FILE* file,char* filename,char* mode){
    errno_t err = fopen_s(&file, filename, mode);

	if (err)
	{
		perror("fopen");
		return(NULL);
	}
    return file;
}

// int alloc_and_write(FILE* file,const char* pattern,Map* map){
// 	char* buffer = (char*)calloc(strlen(pattern),sizeof(char));
// 	sprintf(buffer,pattern,map->size.x,map->size.y,map->level);
// 	fprintf(file,"%s",buffer);
// 	free(buffer);
//     return SUCCESS;
// }


char* int_to_char(int value)
{
    char* buffer = (char*)calloc(5, sizeof(char));

    sprintf(buffer, "%d", value);

    return buffer;
}

int export_map(Map* map, char* fileName)
{
    clock_t start, end;
    start = clock();
    if (map == NULL || fileName == NULL)
        return ERROR;

    FILE* file_data = NULL;
    file_data = open_file(file_data, fileName, "w+");

    const char* header = "{\n\n\"width\": %d,\n\"height\" : %d,\n \"diff\" : %2d\n,\n";
    char* header_buff = (char*)calloc(strlen(header), sizeof(char));
    sprintf(header_buff, header, map->size.x, map->size.y, map->level);
    fprintf(file_data, "%s", header_buff);
    free(header_buff);

    const char* footer = "\"start\":{\"x\":%2d,\"y\":%2d},\"end\":{\"x\":%2d,\"y\":%2d}";
    char* footer_buff = (char*)calloc(strlen(footer), sizeof(char));
    sprintf(footer_buff, footer, map->entry.x, map->entry.y, map->exit.x, map->exit.y);

    int _data_size = 6 * map->size.x * map->size.y + 1;

    char* data_buffer = (char*)calloc(_data_size, sizeof(char));
    int last_mazeblock_index = map->size.y * map->size.x;
    char* default_parsing_string = "%3s,";
    char* _default_parsing_string;

    for (int y = 0; y < map->size.y; y++)
    {
        for (int x = 0; x < map->size.x; x++)
        {
            char c_buffer[6];
            _default_parsing_string = default_parsing_string;

            if ((y + 1) * (1 + x) == last_mazeblock_index)
            {
                _default_parsing_string = "%3s";
            }

            if (0 > sprintf(c_buffer, _default_parsing_string, renderPos(map->data[x][y])))
            {
                printf("\nerror in sprintf");
            }
            if (strcat(data_buffer, c_buffer) == NULL)
            {
                printf("\nerror data");
            }
        }
    }

    char* body = "\n%s,\"data\":[%s]}";
    int body_size = strlen(body) + 2 * strlen(data_buffer) + strlen(footer_buff);
    char* body_buff = (char*)calloc(body_size, sizeof(char));

    if (0 > sprintf(body_buff, body, footer_buff, data_buffer))
    {
        printf("error in sprintf_s");
    }
    free(data_buffer);
    fprintf(file_data, "%s", body_buff);
    free(body_buff);
    free(footer_buff);

    // fprintf(file_data,"%s",footer_buff);
    fclose(file_data);
    end = clock();
    double time_taken = (double)(end - start) / (double)CLOCKS_PER_SEC;
    time_taken *= 1000; // Conversion en ms
    printf("Done Writing in %s in %lfms!\n", fileName, time_taken);
    return 1;
}

int posToMap(int pos){

    if(pos == T_WALL) return T_WALL;
    else if(pos == 7) return D_ROCK;
    return 0;
}

int string_to_int(FILE* file,int seek,int seek_from,int readsize,int count){
    if(!file) return ERROR;
    char buffer[4];
    fseek(file,seek,seek_from);
    fread(buffer,readsize,count,file);
    return strtol(buffer,NULL,10);
}

Map* import_map(Map* map,char* filename){
	if (!filename) return NULL;
	printf("\n[*] Importing map..");
	FILE* fichier_data = NULL;
    fichier_data = open_file(fichier_data,filename,"r+");
   
    int width = string_to_int(fichier_data,14,0,2,1);
    int height = string_to_int(fichier_data,30,0,2,1);
    int diff = string_to_int(fichier_data,46,0,1,1);
    Vec2 size = {width,height};
    map = init_memory(map,size,diff);

    printf("\n%d %d",width,height);
	int _x = string_to_int(fichier_data,89,0,2,1);
    int _y = string_to_int(fichier_data,96,0,2,1);
    Vec2 exit = {_x,_y};
	map->exit = exit;
    printf("\nexit: (%d, %d)", map->exit.x, map->exit.y);

	int* textures = NULL; int* map_data = NULL;
	// texture // data
	fseek(fichier_data,107,0); //68
	int pos;
    
	for(int i = 0;i<height;i++){
		for(int j = 0;j<width;j++){
            pos = posToMap(string_to_int(fichier_data,2,SEEK_CUR,2,1));
			map->data[j][i] = pos;
		}
	}
	fclose(fichier_data);
	printf("\n[*] Import Done");
	return map;
}