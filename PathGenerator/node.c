#include "mapgen.h"
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



void create_node(Node** node,Vec2 pos,Map* map)
{
    node = (Node*)malloc(sizeof(Node*));
    (*node)->pos = pos;
    (*node)->value = getValFromPos(map,pos);
    (*node)->up = NULL;
    (*node)->right = NULL;
    (*node)->down = NULL;
    (*node)->left = NULL;

    //up

    Vec2 _up = pos;
    Vec2 _right = pos;
    Vec2 _down = pos;
    Vec2 _left = pos;

    if(check_pos(map->data[pos.x][pos.y-1])) create_node()
}