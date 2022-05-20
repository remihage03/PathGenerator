#include "stack.h"
#include <stdbool.h>
#include "mapgen.h"

#define STACKOVERFLOW -1

// cr�ation d'une pile
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
		// allocation du tableau de donn�es
>>>>>>> e4d0e7a38533644f3669aa84ba58b3ce0778cfe0
		(*stack)->array = (Vec2*)malloc(sizeof(Vec2) * initialStackSize);
		if (((*stack)->array) != NULL)
		{
			// initialisation des param�tres de la pile
			(*stack)->size = initialStackSize;
			(*stack)->eltsCount = 0;
		}
		else
		{
			// la pile n'a pas pu �tre cr��e, stack vaut NULL
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
// r�cup�re la valeur au sommet de la pile
=======
// r�cup�re la valeur au sommet de la pile
>>>>>>> e4d0e7a38533644f3669aa84ba58b3ce0778cfe0
int pull(Stack* stack, Vec2* value)
{
	if (!isStackEmpty(stack))
	{
		// COMPLETER LE CODE ICI ************************
		*value = stack->array[stack->eltsCount - 1];
		stack->eltsCount--; // On d�cr�mente le nombre d'�l�ments de 1: notre dernier �l�ment peut �tre �cras�

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