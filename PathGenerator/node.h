#include "mapgen.h"
#include <stdlib.h>
//up,right,down,left sont des cases walkables
//si 3 sont nulles == position stuck => retour en arrière
typedef struct node {
    int value;
    Vec2 pos;
    struct node* up;
    struct node* right;
    struct node* down;
    struct node* left;
}Node;

// Bon node sa mère
void create_node(Node** node, Vec2 pos, Map* map);