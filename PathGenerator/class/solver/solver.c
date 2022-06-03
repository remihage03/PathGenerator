#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

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
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap_int(&arr[j], &arr[j + 1]);
}

void sort_by_dist(Vec2* positions,int* pos_dist,int n){
    int i, j;
    for (i = 0; i < n - 1; i++)
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (pos_dist[j] > pos_dist[j + 1]){
                // printf("\n j = %d ; j+1 = %d",pos_dist[j],pos_dist[j + 1]);
                swap_int(&pos_dist[j], &pos_dist[j + 1]);
                swap_vec2(&positions[j],&positions[j + 1]);
            }
}

bool check_player_pos(int _data){
    return (!(_data == T_WALL || _data == D_ROCK));
}

bool check_player_move(Map* map,Vec2 pos,Dir dir){
    Vec2 _pos = {pos.x,pos.y};
    exec_move(&_pos,dir);
    if(!check_player_pos(map->data[_pos.x][_pos.y])) return false;
    if(count_cardinals(map,_pos)==4) return false;
    return true;
}

void add_path_pos(Map* map,Vec2 pos){
    map->path[map->path_count] = pos;
    map->path_count+=1;
}

void remove_path_pos(Map*map, Vec2 pos){
    int i = 0;
    while(i< map->path_count && !isEqual(map->path[i],pos)){
        i++;
    }
    if(isEqual(map->path[i],pos)){
        for(int n = i;n<map->path_count-1;n++){
            map->path[n] = map->path[n+1];
        }
        map->path_count--;
    }
    
}

void draw_path(Map* map){
    for(int i =1;i<map->path_count;i++){
        Vec2 _from = map->path[i-1];
        Vec2 _to = map->path[i];

        if(_from.x != _to.x){
            if(_from.x > _to.x){
                for(int n = _to.x;n<_from.x;n++)
                    map->data[n][_from.y] = 1;
            }
            else{
                for(int n = _from.x;n<_to.x;n++)
                    map->data[n][_from.y] = 1;
            }
        }
        if(_from.y != _to.y){
            if(_from.y > _to.y){
                for(int n = _to.y;n<_from.y;n++)
                    map->data[_from.x][n] = 1;
            }
            else{
                for(int n = _from.y;n<_to.y;n++)
                    map->data[_from.x][n] = 1;
            }   
        }
    }

    
}


bool Solve(Map* map,int X, int Y,Vec2 _from_pos)
{
    // printf("\n %d %d => %d %d",map->exit.x,map->exit.y,X,Y);

    Vec2 pos = {X,Y};

    // print_shard(map,&printPath);


    if (isEqual(map->exit,pos))
    {
        // push(path,map->exit);
        add_path_pos(map,pos);
        return true;
    }


    Dir directions[4] = {DIR_UP,DIR_RIGHT,DIR_DOWN,DIR_LEFT};
    Vec2 tmp_pos[4] = {pos,pos,pos,pos};
    Vec2 new_pos[4] = {pos,pos,pos,pos};
    int new_pos_dist[4];
    int dir_count = 0;

    // getting tmp pos
    for(int n = 0;n<4;n++){
        bool moved = false;
        while(check_player_move(map,tmp_pos[n],directions[n]) && !isEqual(map->exit,tmp_pos[n])){
            exec_move(&tmp_pos[n],directions[n]);
            moved = true;
        }
        if(moved){
            new_pos[dir_count] = tmp_pos[n];
            new_pos_dist[dir_count] = ManDist(tmp_pos[n],map->exit);
            dir_count+=1;
        }
    }
    map->data[X][Y] = PATH;

    add_path_pos(map,pos);

    
    // printf("\nNOT SORTED : ");
    // for(int i = 0;i<dir_count;i++){
    //     printf("\ni : %d - x = %d ; y = %d",i,new_pos[i].x,new_pos[i].y);
    // }

    sort_by_dist(new_pos,new_pos_dist,dir_count);
    // printf("\n SORTED :");
    // for(int i = 0;i<dir_count;i++){
    //     printf("\ni : %d - x = %d ; y = %d - dist : %d",i,new_pos[i].x,new_pos[i].y,ManDist(new_pos[i],map->exit));
    // }

    for(int i = 0;i<dir_count;i++){
        //i petit est la meilleure direction possible
        if(getValFromPos(map,new_pos[i])!=1 && Solve(map,new_pos[i].x,new_pos[i].y,pos))
            return true;
    }
    
    map->data[X][Y] = 0;
    remove_path_pos(map,pos);
    return false;
}

