#include "mapgen.h"
#include <stdlib.h>
//up,right,down,left sont des cases walkables
//si 3 sont nulles == position stuck => retour en arrière
typedef struct node{
	int value;
    Vec2 pos;
	struct node* up;
	struct node* right;
	struct node* down;
	struct node* left;
}Node;

// Bon node sa mère
void create_node(Node** node,Vec2 pos,Map* map)
{
    *node = (Node*)malloc(sizeof(Node*));

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
    exec_move(&_up,DIR_UP);
    Vec2 _right = pos;
    exec_move(&_right,DIR_RIGHT);
    Vec2 _down = pos;
    exec_move(&_down,DIR_DOWN);
    Vec2 _left = pos;
    exec_move(&_left,DIR_LEFT);

    if (check_pos(getValFromPos(map,_up))) create_node(&(*node)->up,_up,map);
    if (check_pos(getValFromPos(map,_right))) create_node(&(*node)->right,_right,map);
    if (check_pos(getValFromPos(map,_down))) create_node(&(*node)->down,_down,map);
    if (check_pos(getValFromPos(map,_left))) create_node(&(*node)->left,_left,map);
}