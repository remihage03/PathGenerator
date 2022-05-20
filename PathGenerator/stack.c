#include "stack.h"
#include <stdbool.h>
#include "mapgen.h"

#define STACKOVERFLOW -1

// crï¿½ation d'une pile
// stack : nom de la pile
// initialStackSize : taille de la pile en nombre de valeurs empilables
void NewStack(Stack** stack, int initialStackSize)
{
	// allocation de la structure pile
	*stack = (Stack*)malloc(sizeof(Stack));
	if (*stack != NULL)
	{
<<<<<<< HEAD
		// allocation du tableau de donnees
=======
		// allocation du tableau de données
>>>>>>> e4d0e7a38533644f3669aa84ba58b3ce0778cfe0
		(*stack)->array = (Vec2*)malloc(sizeof(Vec2) * initialStackSize);
		if (((*stack)->array) != NULL)
		{
			// initialisation des paramï¿½tres de la pile
			(*stack)->size = initialStackSize;
			(*stack)->eltsCount = 0;
		}
		else
		{
			// la pile n'a pas pu ï¿½tre crï¿½ï¿½e, stack vaut NULL
			free(*stack);
			*stack = NULL;
		}
	}
}

// teste si la piles est pleine
bool isStackFull(Stack* stack) {
	return(stack->eltsCount >= stack->size);
}

// teste si la pile est vide
bool isStackEmpty(Stack* stack) {
	return(stack->eltsCount == 0);
}

// pousse une valeur sur la pile
<<<<<<< HEAD
int push(Stack* stack, Vec2 value)
=======
int push(Stack* stack, Vec2 Vec2)
>>>>>>> e4d0e7a38533644f3669aa84ba58b3ce0778cfe0
{
	if (!isStackFull(stack))
	{
		stack->array[stack->eltsCount] = Vec2 ;
		stack->eltsCount++;
		return(0);
	}
	else return(STACKOVERFLOW);
}

<<<<<<< HEAD
// rï¿½cupï¿½re la valeur au sommet de la pile
=======
// récupère la valeur au sommet de la pile
>>>>>>> e4d0e7a38533644f3669aa84ba58b3ce0778cfe0
int pull(Stack* stack, Vec2* value)
{
	if (!isStackEmpty(stack))
	{
		// COMPLETER LE CODE ICI ************************
		*value = stack->array[stack->eltsCount - 1];
		stack->eltsCount--; // On dï¿½crï¿½mente le nombre d'ï¿½lï¿½ments de 1: notre dernier ï¿½lï¿½ment peut ï¿½tre ï¿½crasï¿½

		return(EXIT_SUCCESS);
	}
	return(EXIT_FAILURE);
}

int peek(Stack* stack, Vec2* value)
{
	if (!isStackEmpty(stack))
	{
		// COMPLETER LE CODE ICI ************************
		*value = stack->array[stack->eltsCount - 1];
		return(EXIT_SUCCESS);
	}
	return(EXIT_FAILURE);
}