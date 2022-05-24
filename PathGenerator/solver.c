#include "mapgen.h"
#include "stack.h"
#include "solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//#define DEBUG

typedef struct node{
	int value;
	Vec2 pos;
	struct node* up;
	struct node* right;
	struct node* down;
	struct node* left;
}Node;

int getValFromPos(Map* map, Vec2 pos)
{
	if (!isValid(map, pos)) return -1;

	return map->data[pos.x][pos.y];
}

bool isEqual(Vec2 a, Vec2 b)
{
	return (a.x == b.x && a.y == b.y);
}

bool isInStack(Stack* stack, Vec2 vec)
{
	for (int i = 0; i < stack->eltsCount; i++)
	{
		if (isEqual(stack->array[i], vec))
			return true;
	}
	return false;
}

//void pullNode(Stack* stack, Node node)
//{
//	int idx = 0;
//	bool found = false;
//	for (idx; idx < stack->eltsCount; idx++)
//	{
//		if (stack->array[idx].pos.x == node.pos.x && stack->array[idx].pos.y == node.pos.y)
//		{
//			found = true;
//			break;
//		}
//	}
//
//	if (!found) return;
//
//	for (int i = idx; i < stack->size - 1; i++)
//	{
//		stack->array[i] = stack->array[i + 1];
//	}
//	stack->eltsCount--;
//}

unsigned int ManDist(Vec2 a, Vec2 b)
{
	// * 10 pour avoir plus de précision (en gros 1 chiffre après la virgule mais sans virgule)
	return (unsigned int)(10 * (abs(b.x - a.x) + abs(b.y - a.y))); // Distance Manhattan
}

//unsigned int EuclDist(Vec2 a, Vec2 b) // Heuristic
//{
//	if (a.x == b.x && a.y == b.y) return 0;
//	return (unsigned int)(10 * sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2))); // Distance Euclidienne
//}

bool isValid(Map* map, Vec2 pos)
{
	return (pos.x >= 0 && pos.x < map->size.x && pos.y >= 0 && pos.y < map->size.y);
}

bool isWalkable(Map* map, Vec2 pos)
{
	return (isValid(map, pos) && map->data[pos.x][pos.y] != T_WALL && map->data[pos.x][pos.y] != D_ROCK);
}

int countNeighbors(Map* map, Vec2 pos, Stack* stack) // Compte les voisins aux 4 points cardinaux de pos
{
	if (!map) return ERROR;

	int nbNeighbors = 0;

	for (int y = pos.y - 1; y <= pos.y + 1; y++)
	{
		Vec2 _pos = { pos.x, y };
		if (y != pos.y && isWalkable(map, pos) &&  !isInStack(stack, pos))
			nbNeighbors++;
	}

	for (int x = pos.x - 1; x <= pos.x + 1; x++)
	{
		Vec2 _pos = { x, pos.y };
		if (x != pos.x && isWalkable(map, _pos) && !isInStack(stack, pos))
			nbNeighbors++;
	}

	return nbNeighbors;
}

bool canMove(Map* map, Vec2 pos, Dir dir)
{
	exec_move(&pos, dir);
	return isWalkable(map, pos);
}

Vec2 slide_back(Map* map,Stack* path,Vec2 pos,Dir from){

	Vec2 _pos = pos;
	Vec2 tmp;
	while(canMove(map, _pos, -from) && !isEqual(map->exit,pos)){
		if (isInStack(path, _pos))
			pull(path, &tmp);
		exec_move(&_pos, -from);
	}
	return _pos;
}

bool isStuck(Map* map,Vec2 init_pos,Stack* path){
	bool stuck = true;
	Vec2 pos = init_pos;
	Vec2 neighbors[4] = {pos, pos, pos, pos};
	exec_move(&neighbors[0], DIR_UP);
	exec_move(&neighbors[1], DIR_RIGHT);
	exec_move(&neighbors[2], DIR_DOWN);
	exec_move(&neighbors[3], DIR_LEFT);
	for(int i = 0;i<4;i++){
		if (isWalkable(map, neighbors[i]) && countNeighbors(map, neighbors[i], path) != 0)
			stuck = false;
	}
	return stuck;
}


//return the direction to take when reverse
Dir find_counter_dir(Vec2 from,Vec2 target)
{
	if(from.x != target.x){
		if(from.x > target.x) return DIR_LEFT ;
		else return DIR_RIGHT;
	}
	else{
		if(from.y > target.y) return DIR_UP;
		else return DIR_DOWN;
	}
	
}

//move until rock or exit vec2
void move_until_rock(Map* map,Stack* path,Vec2* pos,Dir dir,bool simulation)
{
	while(canMove(map,*pos,dir) && !isEqual(*pos, map->exit))
	{
		exec_move(pos,dir);
		if(!isInStack(path,*pos) && simulation)
			push(path,*pos);
	}
}

//tri un tab de direction en fonction des distances associées
Dir* trier_dir(int distances[4]){
	Dir *sorted_dir = malloc(sizeof(Dir)*4);
 	// sorted_dir = {DIR_UP,DIR_RIGHT,DIR_DOWN,DIR_LEFT};
	sorted_dir[0] = DIR_UP;
	sorted_dir[1] = DIR_RIGHT;
	sorted_dir[2] = DIR_DOWN;
	sorted_dir[3] = DIR_LEFT;
	// for(int i = 0;i<4;i++){
	// 	printf("\ndistance : %d -> dir : %d",distances[i],sorted_dir[i]);
	// }
	int tmp;
	Dir tmp_dir;
	for (int i = 0; i < 4; i++) // Bubble sort (un peu lent ?)
    {
        for (int j = 0; j < 4 - i - 1; j++)
        {
            if (distances[j] > distances[j + 1])
            {
                // Swap
                tmp = distances[j];
				tmp_dir = sorted_dir[j];
                distances[j] = distances[j + 1];
				sorted_dir[j] = sorted_dir[j+1];
                distances[j + 1] = tmp;
				sorted_dir[j+1]= tmp_dir;
            }
        }
    }

	
	return sorted_dir;
}

int megaSolver4000(Map* map,Stack* path,Vec2 pos){
	if(isEqual(pos,map->exit)) return SUCCESS;

	Dir directions[4] = {DIR_UP,DIR_RIGHT,DIR_DOWN,DIR_LEFT};
	Vec2 neighbors[4] = {pos, pos, pos, pos};

	move_until_rock(map,path,&neighbors[0],DIR_UP,1);
	move_until_rock(map,path,&neighbors[1],DIR_RIGHT,1);
	move_until_rock(map,path,&neighbors[2],DIR_DOWN,1);
	move_until_rock(map,path,&neighbors[3],DIR_LEFT,1);

	printf("\npos : x = %d, y = %d",pos.x,pos.y);

	int distances[4];
	for(int i = 0;i<4;i++){
		distances[i] = ManDist(neighbors[i], map->exit);
	}

	Dir *sorted_dir;
	sorted_dir = trier_dir(distances);

	// for(int i = 0;i<4;i++){
	// 	if(!isWalkable(map,neighbors[i])) continue;
	// 	else return megaSolver4000(map,path,neighbors[i]);
	// }
	
	return ERROR;

}

int megaSolver3000(Map* map) {
	if (!map) return ERROR;


	Dir directions[4] = {DIR_UP,DIR_RIGHT,DIR_DOWN,DIR_LEFT};

	// Va contenir notre chemin
	Stack* path;
	int res = map->size.x * map->size.y;
	NewStack(&path, res);

	Stack* blocked;
	NewStack(&blocked, res);

	Vec2 pos = map->entry, oldPos = pos;
	push(path, pos);

	Stack* last_good;
	NewStack(&last_good, res);
	push(last_good,pos);
	Dir lastdir = DIR_DOWN;
	unsigned int ite = 0;

	while (!isEqual(pos, map->exit) && ite < res) {
		ite++;
		int dir_inc = 0;
		Dir dir = DIR_DOWN;
		bool has_moved = false;
		// Coords des 4 voisins cardinaux
		Vec2 neighbors[4] = {pos, pos, pos, pos};
		//remplacer par un slide move pour comparer la distance ?
		
		move_until_rock(map,path,&neighbors[0],DIR_UP,1);
		move_until_rock(map,path,&neighbors[1],DIR_RIGHT,1);
		move_until_rock(map,path,&neighbors[2],DIR_DOWN,1);
		move_until_rock(map,path,&neighbors[3],DIR_LEFT,1);
		// exec_move(&neighbors[0], DIR_UP);
		// exec_move(&neighbors[1], DIR_RIGHT);
		// exec_move(&neighbors[2], DIR_DOWN);
		// exec_move(&neighbors[3], DIR_LEFT);

		//création du tableau de distance en fonction des voisins aux 4 pts card
		int distances[4];
		for(int i = 0;i<4;i++){
			distances[i] = ManDist(neighbors[i], map->exit);
		}

		Dir *sorted_dir;
		sorted_dir = trier_dir(distances);
		for(int i = 0;i<4;i++){
			printf("\n direction avec la dist minimale : %d",sorted_dir[i]);
			// if (!isWalkable(map, neighbors[i]) || countNeighbors(map, neighbors[i], path) == 0 || isInStack(blocked, neighbors[i]))
			// 	continue;
		}
		// break;

		// Find closest neighbor
		// unsigned int dst = INT_MAX;
		// for (int i = 0; i < 4; i++) {
		// 	int newDst = ManDist(neighbors[i], map->exit);
		// 	// print_shard(map, &printMapData);
		// 	// printf("\n     -> blocked : x = %d, y = %d",osef.x,osef.y);

		// 	if (!isWalkable(map, neighbors[i]) || countNeighbors(map, neighbors[i], path) == 0 || isInStack(blocked, neighbors[i]))
		// 		continue;

		// 	printf("\ni = %d ; pos : x = %d, y = %d - dist = , newDist = %d",i,neighbors[i].x,neighbors[i].y,newDst);

		// 	if(newDst<dst){
		// 		dst = newDst;
		// 		dir = directions[i];
		// 	}
			
		// }

		// Tant qu'on a pas rencontré un obstacle, on continue de glisser
		dir = sorted_dir[dir_inc];
		while (canMove(map, pos, dir) && !isEqual(pos, map->exit)) {
			exec_move(&pos, dir);
			if (!isInStack(path, pos))
				push(path, pos);
			has_moved = true;
		}

		//si on est bloqué dans cette position sans être en sortie
		// if (countNeighbors(map, pos, path) == 0 && !isEqual(pos, map->exit)) {
		if(isStuck(map,pos,path)){
			Vec2 tmp;
			Vec2 last;
			pull(last_good,&last);
			Dir counter = find_counter_dir(pos,last);
			// Dir counter = -dir;
			// printf("\nstuck -> coutner dir : %d",counter);
			while(!isEqual(last,pos)){
				pull(path, &pos);
				// exec_move(&pos, counter);
			}
			dir_inc+=1;
			dir_inc%=4;
			dir = sorted_dir[dir_inc];
			// push(path,pos);
			// exec_move(&pos, -dir);
			// pull(path, &tmp);
			// printf("\nsliding : x = %d, y = %d",pos.x,pos.y);
		}

		
		if(has_moved){
			push(last_good,pos);
		}
		// oldPos = pos;


		//if (countNeighbors(map, pos) <= 2 && !isEqual(pos, map->exit))
		//{
		//	pull(path, &pos);
		//	push(blocked, pos);
		//	oldPos = pos;
		//}
	}

	Vec2 tmp;
	peek(path, &tmp);
	if (isEqual(tmp, map->exit)) {
		printf("\n[+] Path found !\n");
	}
	else {
		printf("\n[+] Unable to find a path.\n");
	}

	// On crée le chemin
	while (!isStackEmpty(path)) {
		pull(path, &tmp);
		map->data[tmp.x][tmp.y] = -1;
	}
	print_shard(map, &printPath);

	return SUCCESS;
}