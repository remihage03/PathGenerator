#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include "list.h"

// #define VERSION 22.0
// #define STUDENTFORM


// crée une nouvelle liste chainée unilatère vide et renvoie un pointeur sur cette liste
LinkedList* NewLinkedList() {
	LinkedList* tmp;
	tmp = (LinkedList*)malloc(sizeof(LinkedList));
	if (tmp != NULL) {
		tmp->head = NULL;
		tmp->tail = NULL;
		tmp->size = 0;
	}
	return tmp;
}

// crée un nouveau maillon qui contient la personne passée en paramètre
SingleLinkedListElem* NewLinkedListItem(Node node) 
{
	SingleLinkedListElem* tmp;
	tmp = (SingleLinkedListElem*)malloc(sizeof(SingleLinkedListElem));
	if (tmp != NULL) 
	{
		tmp->data = node;
		tmp->next = NULL;
	}
	return(tmp);
}

// renvoie un pointeur sur l'élément en ième position dans la liste
SingleLinkedListElem* GetElementAt(LinkedList* Liste, int i) 
{	
	if ((Liste == NULL) || (i < 0) || (i >= Liste->size)) return(NULL);
	if (i == 0) return(Liste->head);
	if (i == Liste->size - 1) return(Liste->tail);

	int currentIndex = 0;
	SingleLinkedListElem* element;
	element = Liste->head;

	while (currentIndex != i && element != NULL) 
	{
		element = element->next;
		currentIndex++;
	}

	return(element);
}

// ajout d'un maillon à la fin d'une Single list
// renvoie 1 si le maillon a été ajouté, 0 sinon
int AppendNewElemToSingleLinkedList(LinkedList*list,SingleLinkedListElem* elem)
{
	if (list == NULL|| elem == NULL|| elem->next != NULL) return 0;// si la liste est vide
	
	if (list->tail == NULL) 
	{
		list->tail= elem;
		list->head= elem;
		list->size = 1;
	}
	else // on ajoute l'élément en fin
	{
		list->tail->next= elem;
		list->tail = elem;
		list->size += 1;
	}
	return 1;
}

int DeleteSingleLinkedListElement(LinkedList* list, SingleLinkedListElem* elem)
{
	// on peut traiter un certain nombre de cas d'erreur
	
	//La liste n'existe pas
	if (list == NULL) return(0); 
	
	// liste vide ou anomalie
	if ((list->head == NULL) || (list->tail == NULL)) return(0); 
	
	// anomalie sur la taille
	if ((list->head == list->tail) && (list->size != 1)) return(0);
	
	// pas d'élément dans la liste ou elem invalide
	if ((list->size == 0) || (elem== NULL)) return(0); 
	
	// initialisation d'un pointeur sur l'élément courant
	SingleLinkedListElem*tmp= list->head;
	
	// previous désigne l'élément qui précède l'élément courant
	SingleLinkedListElem*previous= NULL;

	// l'élément est en tête et en queue, il y a donc 1 seul élément dans la liste
	if ((elem == list->head) && (elem == list->tail)) 
	{
		list->head= NULL;
		list->tail= NULL;
		list->size = 0;
		free(elem);
		return(1);
	}
	
	// il est en tête, on supprime la tête
	if (elem == list->head)  
	{
		list->head = elem->next;
		list->size--;
		free(elem);
		return(1);
	}

	// Recherche du maillon dans le reste de la liste chaînée
	while((tmp!= NULL) && (tmp!= elem)) 
	{
		previous = tmp;
		tmp = tmp->next;
	}
	
	// s'il est en queue, on supprime la queue
	if ((previous != NULL) && (tmp== elem) && (tmp->next==NULL)) 
	{
		list->tail= previous;
		previous->next= NULL;
		list->size--;
		free(elem);
		return(1);
	}
	
	// s'il est au milieu, on supprime l'élément
	if ((previous !=NULL) && (tmp == elem) && (tmp->next!=NULL))
	{
		previous->next = elem->next;
		list->size--;
		free(elem);
		return(1);
	}
	// l'élément recherché n'a pas été trouvé
	return(0);  
}

int DisplayList(LinkedList* Liste)
{
	for (SingleLinkedListElem* elt = Liste->head; elt; elt = elt->next)
	{
		printf("%5d ", elt->data);
	}

	printf("\n");
}

int insertElemAtLinkedListHead(LinkedList* list, SingleLinkedListElem* elem)
{
	if (list == NULL || elem == NULL || elem->next != NULL) return 0;// si la liste est vide

	if (list->head != NULL)
	{
		SingleLinkedListElem* tmp = list->head;
		elem->next = tmp;
		list->head = elem;

		list->size++;
	}

	return 1;
}

bool isEmpty(LinkedList* Liste)
{
	if (!Liste) return;

	return (Liste->size == 0);
}

bool isInLinkedList(LinkedList* Liste, Node* node)
{
	if (!Liste || !node) return;

	for (SingleLinkedListElem* elt = Liste->head; elt; elt = elt->next)
	{
		if (elt->data.pos.x == node->pos.x && elt->data.pos.y == node->pos.y)
			return true;
	}
	return false;
}