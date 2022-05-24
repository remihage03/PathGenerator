#pragma once
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
void create_children(Node* node,Map* map,int dir_index);

Node* create_master_node(Node* node,Vec2 pos,Map* map);
// void create_node(Node* node, Vec2 pos, Map* map,Dir master);
void slide_move(Map* map, Vec2* pos, Dir dir);
void explore_graph(Node* node);
int count_valid_path(Node* node);