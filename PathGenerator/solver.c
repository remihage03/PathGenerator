#include "mapgen.h"
#include "stack.h"

typedef struct Path{
    int move;
    int cel;
}Path;


bool vec2_equals(Vec2 first,Vec2 second){
    return (first.x == second.x && first.y == second.y) ? true: false;
}

bool slide_move(Map* map,Vec2* pos,Dir dir,Stack* path){
    bool move_flag = false;
    while(check_move(map,*pos,dir))
    {
        exec_move(pos,dir); // on avance
        // printf("\n POS : %d %d",pos->x,pos->y);
        // push(path,*pos); // on push la valeur
        map->data[pos->x][pos->y] = 1; //on update la map
        move_flag = true;
        if(vec2_equals(*pos,map->exit)) break;
    }
    return move_flag;
}

bool slide_move_print(Map* map,Vec2* pos,Dir dir){
    bool move_flag = false;
    while(check_move(map,*pos,dir))
    {
        exec_move(pos,dir); // on avance
        map->data[pos->x][pos->y] = 1; //on update la map
        move_flag = true;
        if(vec2_equals(*pos,map->exit)) break;
    }
    return move_flag;
}

bool check_slide_move(Map* map,Vec2 from,Dir dir){
    Vec2 _from = from;
    return slide_move(map,&_from,dir,NULL);
}

void print_directions_val(int* directions){
    printf("\nUP : %d\nRIGHT : %d\nDOWN : %d\nLEFT : %d",directions[0],directions[1],directions[2],directions[3]);
}


Vec2 pull_until_last(Stack* path,Vec2 current,Vec2 last,Map* map){
    if(vec2_equals(current,last)) return current;
    map->data[current.x][current.y] = 0;
    pull(path,&current);
    return pull_until_last(path,current,last,map);
}

Dir* valid_direction(Map* map,Vec2 pos,Dir current)
{
    
}



//je dois ajouter chaque point où je tourne à la stack
// et si au final je suis bloqué je return

int test_path(Stack* path,Map* map, Dir last,int ite){
    Vec2 last_pivot;
    peek(path,&last_pivot);
    // printf("\n from %d %d",last_pivot.x,last_pivot.y);
    if(vec2_equals(map->exit,last_pivot)) return SUCCESS;
    
    printf("\nite : %d",ite);
    bool has_moved = false;


    for(int i = -2;i<3;i++){
        if(i != 0 && i != -last){
            has_moved = false;
            Vec2 _last_pivot = last_pivot;
            // printf("\ni : %d",i);
            if(!check_slide_move(map,last_pivot,i)){
                // pull(path,NULL);
                return ERROR;
            }
            if(slide_move(map,&last_pivot,i,path))
            {
                has_moved = true;
                last = i;
                push(path,last_pivot);
                return test_path(path,map,last,ite+1);
                // if(test_path(path,map,last,ite+1) == ERROR) pull(path,NULL); 
                // return test_path(path,map,last,ite+1);
            }
            // printf("\ni = %d from %d %d to %d %d",i,_last_pivot.x,_last_pivot.y,last_pivot.x,last_pivot.y);
            // if(has_moved){
            // }
            
        }
    }
    //si on a pas bougé
    if(!has_moved) return ERROR;
    Vec2 osef;
    printf("\n changement ! ");
    // pull(path,&osef);
    return test_path(path,map,last,ite+1);
    // print_shard(map,&printPath);
}



Dir render_distance(Map* map, Vec2 from,Vec2 to)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    if(dy == 0)
    {
        if(dx > 0) return DIR_RIGHT;
        return DIR_LEFT;
    }
    else if(dx == 0)
    {
        if(dy > 0) return DIR_DOWN;
        return DIR_UP;
    }
}

void renderStack(Stack* path,Map* map)
{
    Vec2 from;
    Vec2 to;
    Vec2 tmp;
    pull(path,&to);
    while(!isStackEmpty(path))
    {
        pull(path,&from);
        Dir l_dir = render_distance(map,from,to);
        tmp = from;
        map->data[to.x][to.y] = 1;
        slide_move_print(map,&tmp,l_dir);
        to = from;
    }
}

void printStack(Stack* path,Map* map){
    Vec2 tmp;
    if(isStackEmpty(path)){
        return;
    }
    pull(path,&tmp);
    map->data[tmp.x][tmp.y] = 1;
    printStack(path,map);
    // printf("\nx = %d ; y = %d",tmp.x,tmp.y);
}

//le path final est une liste chainée de vec2
//
Vec2* solve_maze(Vec2* path,char* maze_file){
    Map* map = NULL;
    map = import_map(map,maze_file);
    print_shard(map,&printPath);
    int max_dist = map->size.x*map->size.y;
    int *directions = (int*)malloc(sizeof(int)*4);
    Vec2 last_turn;
    int move_since_turn = 0;
    Vec2 current_pos = map->entry;
    Stack* path_stack = NULL;
    NewStack(&path_stack,max_dist);
    push(path_stack,map->entry);


    if(test_path(path_stack,map,-1,0) == SUCCESS){
        printf("\nsuccessful!");
    }

    print_shard(map,&printPath);

    // Vec2 tmp;
    // printStack(path_stack,map);
    renderStack(path_stack,map);
    print_shard(map,&printPath);

    // for(int i = 0;i<path_stack->eltsCount;i++){
        // pull(path_stack,&tmp);
        // printf("\ni : %d, x : %d, y : %d",i,tmp.x,tmp.y);
    // }

    // printf("\n%d %d",current_pos.x,current_pos.y);
    // exec_solve_move(map,&current_pos,2);
    // printf("\n%d %d",current_pos.x,current_pos.y);

    // for(int i = 0;i<max_dist;i++){
    //     directions = valid_directions(map,map->entry);
    //     // print_directions_val(directions);
    // }

    return NULL;
}