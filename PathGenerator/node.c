#include "mapgen.h"
#include "node.h"
#include "solver.h"
#include <stdlib.h>


void slide_move(Map* map, Vec2* pos, Dir dir)
{
    while (canMove(map, *pos, dir))
    {
        exec_move(pos, dir);
    }
}

// Bon node sa mère
void create_node(Node** node,Vec2 pos,Map* map)
{
    *node = (Node*)malloc(sizeof(Node));
    if(!(*node)){
        printf("\nerror in malloc of node pos x = %d, y = %d",pos.x,pos.y);
    }

    // Si malloc échoue
    if (*node == NULL)
    {
        free(*node);
        *node = NULL;
        return;
    }

    (*node)->pos = pos;
    (*node)->value = getValFromPos(map,pos);
    (*node)->up = NULL;
    (*node)->right = NULL;
    (*node)->down = NULL;
    (*node)->left = NULL;

    Vec2 _up = pos;
    slide_move(map, &_up,DIR_UP);
    Vec2 _right = pos;
    slide_move(map, &_right,DIR_RIGHT);
    Vec2 _down = pos;
    slide_move(map, &_down,DIR_DOWN);
    Vec2 _left = pos;
    slide_move(map, &_left,DIR_LEFT);

    if (check_pos(getValFromPos(map,_up))) create_node(&(*node)->up,_up,map);
    if (check_pos(getValFromPos(map,_right))) create_node(&(*node)->right,_right,map);
    if (check_pos(getValFromPos(map,_down))) create_node(&(*node)->down,_down,map);
    if (check_pos(getValFromPos(map,_left))) create_node(&(*node)->left,_left,map);
}