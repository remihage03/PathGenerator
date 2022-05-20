#pragma once
#include "solver.h"

typedef struct SingleLinkedListElem {
	Node data;
	struct SingleLinkedListElem* next;
}SingleLinkedListElem;

typedef struct LinkedList {
	SingleLinkedListElem* head;
	SingleLinkedListElem* tail;
	int size;
}LinkedList;


LinkedList* NewLinkedList();

SingleLinkedListElem* NewLinkedListItem(Node node);

// renvoie EXIT_SUCCESS si le maillon a été ajouté, EXIT_FAILURE sinon
int AppendNewElemToSingleLinkedList(LinkedList* list, SingleLinkedListElem* elem);

int DeleteSingleLinkedListElement(LinkedList* liste, SingleLinkedListElem* element);

int DisplayList(LinkedList* Liste);

SingleLinkedListElem* GetElementAt(LinkedList* Liste, int position);

bool isEmpty(LinkedList* Liste);

bool isInLinkedList(LinkedList* Liste, Node* node);

int insertElemAtLinkedListHead(LinkedList* list, SingleLinkedListElem* elem);