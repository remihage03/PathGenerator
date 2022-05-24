#include "mapgen.h"
#include "node.h"
#include "solver.h"
#include <stdlib.h>


void slide_move(Map* map, Vec2* pos, Dir dir)
{
    while (canMove(map, *pos, dir)){
        exec_move(pos, dir);
    }
}

// Bon node sa mère
void create_node(Node* node,Vec2 pos,Map* map,Dir master)
{
    node = (Node*)malloc(sizeof(Node));

    // Si malloc échoue
    if (node == NULL)
    {
        printf("\nerror in malloc of node pos x = %d, y = %d",pos.x,pos.y);
        free(node);
        node = NULL;
        return;
    }
    printf("\npos : x = %d, y = %d",pos.x,pos.y);
    (node)->pos = pos;
    (node)->value = getValFromPos(map,pos);
    (node)->up = NULL;
    (node)->right = NULL;
    (node)->down = NULL;
    (node)->left = NULL;

    Vec2 _up = pos;
    slide_move(map, &_up,DIR_UP);
    Vec2 _right = pos;
    slide_move(map, &_right,DIR_RIGHT);
    Vec2 _down = pos;
    slide_move(map, &_down,DIR_DOWN);
    Vec2 _left = pos;
    slide_move(map, &_left,DIR_LEFT);

    if(check_pos(getValFromPos(map,_up)) && !isEqual(_up,pos) && master != DIR_UP) 
        create_node(node->up,_up,map,node);
    if(check_pos(getValFromPos(map,_right)) && !isEqual(_right,pos)&& master != DIR_RIGHT) 
        create_node(node->right,_right,map,node);
    if(check_pos(getValFromPos(map,_down)) && !isEqual(_down,pos) && master != DIR_DOWN) 
        create_node(node->down,_down,map,node);
    if(check_pos(getValFromPos(map,_left)) && !isEqual(_left,pos)&& master != DIR_LEFT) 
        create_node(node->left,_left,map,node);


}


//return count of children directions : si haut/bas/droite dispo => count = 3
int count_valid_path(Node* node)
{
    int count = 0;
    if(node->down) count++;
    if(node->left) count++;
    if(node->right) count++;
    if(node->up) count++;
    return count;
}

void explore_graph(Node* node) 
{
    printf("\n node : pos x = %d, y = %d",node->pos.x,node->pos.y);
    if(node->right){
    // printf("\n     node right : x = %d, y = %d",node->right->pos.x,node->right->pos.y);

    }
    if(node->right) explore_graph(node->right);
    if(node->up) explore_graph(node->up);
    if(node->down) explore_graph(node->down);
    if(node->left) explore_graph(node->left);
    
    // if(count_valid_path(node) <3){
        // printf("error zebi");
        // return;
    // }

    // Node* tmp = node;
    
}