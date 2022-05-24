#include "mapgen.h"
#include <stdlib.h>
//up,right,down,left sont des cases walkables
//si 3 sont nulles == position stuck => retour en arri�re
typedef struct node {
    int value;
    Vec2 pos;
    struct node* up;
    struct node* right;
    struct node* down;
    struct node* left;
}Node;

// Bon node sa m�re
void create_node(Node** node, Vec2 pos, Map* map);