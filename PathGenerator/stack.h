#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <stdbool.h>
#include "mapgen.h"
#include "solver.h"

#define STACKOVERFLOW -1

typedef struct {
	Node* array;
	int size;
	int eltsCount;
} Stack;


void NewStack(Stack** stack, int initialStackSize);
bool isStackFull(Stack* stack);
bool isStackEmpty(Stack* stack);
int push(Stack* stack, Node value);
int pull(Stack* stack, Node* value);
int peek(Stack* stack, Node* value);