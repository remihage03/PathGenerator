#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include "../Vec2/Vec2.h"
#include "../mapgen/mapgen.h"


// Define the maximum number of vertices in the graph
#define N 6
 
#define explored 9


void swap_int(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++) // Last i elements are already in place
            if (arr[j] > arr[j + 1])
                swap_int(&arr[j], &arr[j + 1]);
}

void sort_by_dist(Vec2* positions,int* pos_dist,int n) 
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++) // Last i elements are already in place
        {
            if (pos_dist[j] > pos_dist[j + 1]) {
                // printf("\n j = %d ; j+1 = %d",pos_dist[j],pos_dist[j + 1]);
                swap_int(&pos_dist[j], &pos_dist[j + 1]);
                swap_vec2(&positions[j], &positions[j + 1]);
            }
        }
    }
}

bool check_player_pos(int _data){
    return (_data != D_ROCK && _data != T_WATER);
}

bool check_player_move(Map* map,Vec2 pos,Dir dir){
    Vec2 _pos = {pos.x,pos.y};
    exec_move(&_pos, dir);
    if(!isValid(map, _pos) || !check_player_pos(map->data[_pos.x][_pos.y]) || count_cardinals(map,_pos) == 4) return false;
    return true;
}

void add_path_pos(Map* map,Vec2 pos){
    map->path[map->path_count] = pos;
    map->path_count+=1;
}

void remove_path_pos(Map*map, Vec2 pos,Vec2 from){
    int i = 0;
    while(i < map->path_count && !isEqual(map->path[i], pos)){
        i++;
    }
    if(isEqual(map->path[i],pos)){
        // for(int n = i;n<map->path_count-1;n++){
            // map->path[n] = map->path[n+1];
        // }
        // map->path_count--;
        map->path_count = i + 1;
        map->path[i] = from;

    }
    else{
        printf("\nerror in remove");
    }
    
}

// Draw path from turns
void draw_path(Map* map){
    //for(int i = 0;i<map->path_count;i++){
    //    print_vec2(map->path[i]);
    //}

    for(int i = 1; i < map->path_count; i++){
        Vec2 _from = map->path[i - 1];
        Vec2 _to = map->path[i];

        if (isEqual(_from, _to)) continue;

        if (_from.x != _to.x)
        {
            int n = (_from.x > _to.x) ? _to.x : _from.x;
            int cmp = (_from.x > _to.x) ? _from.x : _to.x;

            for (n; n < cmp; n++) {
                map->data[n][_from.y] = PATH;
            }
        }
        else if (_from.y != _to.y)
        {
            int n = (_from.y > _to.y) ? _to.y : _from.y;
            int cmp = (_from.y > _to.y) ? _from.y : _to.y;

            for (n; n < cmp; n++) {
                map->data[_from.x][n] = PATH;
            }
        }

        // Ancien code r???mi
        //if (_from.x > _to.x) {
        //    for (int n = _to.x; n < _from.x; n++)
        //        map->data[n][_from.y] = PATH;
        //}
        //else {
        //    for (int n = _from.x; n < _to.x; n++)
        //        map->data[n][_from.y] = PATH;
        //}

        //if (_from.y > _to.y) {
        //    for (int n = _to.y; n < _from.y; n++)
        //        map->data[_from.x][n] = PATH;
        //}
        //else {
        //    for (int n = _from.y; n < _to.y; n++)
        //        map->data[_from.x][n] = PATH;
        //}

        // Ancien code sacha
        //if(_from.x != _to.x){
        //    if(_from.x > _to.x){
        //        for(int n = _to.x; n<_from.x; n++)
        //            map->data[n][_from.y] = PATH;
        //    }
        //    else{
        //        for(int n = _from.x;n<_to.x;n++)
        //            map->data[n][_from.y] = PATH;
        //    }
        //}
        //if(_from.y != _to.y){
        //    if(_from.y > _to.y){
        //        for(int n = _to.y;n<_from.y;n++)
        //            map->data[_from.x][n] = PATH;
        //    }
        //    else{
        //        for(int n = _from.y;n<_to.y;n++)
        //            map->data[_from.x][n] = PATH;
        //    }   
        //}
    }
    map->data[map->exit.x][map->exit.y] = PATH;
}

Vec2 slide(Map* map,Vec2 init_pos,Dir dir){
    Vec2 pos = {init_pos.x,init_pos.y};
    while(check_player_move(map,pos,dir) && !isEqual(map->exit,pos)){
        exec_move(&pos,dir);
    }
    // printf("\n init pos : %d %d => to pos : %d %d",init_pos.x,init_pos.y,pos.x,pos.y);
    return pos;
}

void stack_move(Map* map,Vec2 pos)
{

    // for(int d = 0;d<deep;d++){

    // }
    int deep = 5;
    
    Dir directions[4] = {DIR_UP,DIR_RIGHT,DIR_DOWN,DIR_LEFT};

    Vec2** new_pos = NULL;
    new_pos = (Vec2**)calloc(deep, sizeof(Vec2*)); // Malloc des x
    for(int i = 0;i<deep;i++){
        new_pos[i] = (Vec2*)calloc(4, sizeof(Vec2));
    }
   
    for(int i = 0;i<4;i++){
        new_pos[0][i] = pos;
    }

    for(int deep_c = 1;deep_c<deep;deep_c++){
        for(int d = 0;d<4;d++){
            Vec2 buffer = slide(map,new_pos[deep_c-1][d],directions[d]);
            if(!isEqual(buffer,new_pos[deep_c-1][d])){
                new_pos[deep_c][d] = buffer;
                printf("\n deep : %d - x = %d ; y = %d",deep_c,new_pos[deep_c][d].x,new_pos[deep_c][d].y);
            }
        }
    }



    // for(int dir = 0;dir<4*deep;dir++){
    //     int tmp_deep = (deep_c <1) ? 0:deep_c-1;
    //     nouvelles_pos[dir+4*deep_c] = slide(map,nouvelles_pos[dir+4*tmp_deep],directions[dir]);
    //     stacked_dist[dir+4*deep_c] = ManDist(nouvelles_pos[dir+4*deep_c],map->exit);
    // }
    // for(int i = deep-4-1;i<4*deep;i++){
    //     printf("\n deep = 3 ; imod4 = %d, x= %d, y=%d",i%4,nouvelles_pos[i].x,nouvelles_pos[i].y);
    // }

}



bool Solve(Map* map,int X, int Y,Vec2 _from_pos)
{
    Vec2 pos = {X,Y};
    if (isEqual(map->exit,pos))
    {
        add_path_pos(map,pos);
        // printf("\nSolved in %d move",count);
        return true;
    }

    Dir directions[4] = {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
    Vec2 tmp_pos[4] = {pos, pos, pos, pos};
    Vec2* new_pos = tmp_pos;
    int new_pos_dist[4];
    int dir_count = 0;

    // getting tmp pos
    for(int n = 0; n < 4; n++)
    {
        bool moved = false;
        while(check_player_move(map,tmp_pos[n], directions[n]) && !isEqual(map->exit, tmp_pos[n])){
            exec_move(&tmp_pos[n],directions[n]);
            moved = true;
        }
        if(moved && !isEqual(_from_pos, tmp_pos[n]) && getValFromPos(map, tmp_pos[n]) != 1){
            new_pos[dir_count] = tmp_pos[n];
            new_pos_dist[dir_count] = ManDist(tmp_pos[n], map->exit);
            dir_count++;
        }
    }
    map->data[X][Y] = PATH;

    add_path_pos(map, pos);
    sort_by_dist(new_pos,new_pos_dist, dir_count);

    for(int i = 0;i < dir_count; i++){
        //i petit est la meilleure direction possible
        // printf("\nteststt");

        // int ret_val = getValFromPos(map,new_pos[i]);
        if(Solve(map, new_pos[i].x, new_pos[i].y, pos))
            return true;
    }
    
    map->data[X][Y] = T_ICE;
    remove_path_pos(map, pos ,_from_pos);
    return false;
}



int _2d_to_1d(Map* map,int x,int y){
    return (map->size.x*x+y);
}

