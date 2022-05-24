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
    (*node)->value = map->data[pos.x][pos.y];
    (*node)->up = NULL;
    (*node)->right = NULL;
    (*node)->down = NULL;
    (*node)->left = NULL;
}