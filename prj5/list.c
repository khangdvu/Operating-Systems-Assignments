//Linked list module provided by Professor Keval Vora of SFU, modified by Khang Vu.

#include "list.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h> 
static _Bool doSinglePassOnSort(struct nodeStruct **headRef);
static void swapElements(struct nodeStruct **previous, struct nodeStruct *nodeA, struct nodeStruct *b);


/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value size. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int size, int from_index, int to_index)
{
	struct nodeStruct *pNode = malloc(sizeof(struct nodeStruct));
	if (pNode != NULL) {
		pNode->size = size;
		pNode->from_index = from_index;
		pNode->to_index = to_index;
	}
	return pNode;
}

/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node)
{
	node->next = *headRef;
	*headRef = node;
}

/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node)
{
	node->next = NULL;

	// Handle empty list
	if (*headRef == NULL) {
		*headRef = node;
	}
	else {
		// Find the tail and insert node
		struct nodeStruct *current = *headRef;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
	}
}

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head)
{
	int count = 0;
	struct nodeStruct *current = head;
	while (current != NULL) {
		current = current->next;
		count++;
	}
	return count;
}

/*
 * Return the first node holding the value size, return NULL if none found
 */
struct nodeStruct* List_findNodebySize(struct nodeStruct *head, int size)
{
	struct nodeStruct *current = head;
	while (current != NULL) {
		if (current->size == size) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

struct nodeStruct* List_findNodeby_fromIndex(struct nodeStruct *head, int from_index)
{
	struct nodeStruct *current = head;
	while (current != NULL) {
		if (current->from_index == from_index) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

struct nodeStruct* List_findNodeby_toIndex(struct nodeStruct *head, int to_index)
{
	struct nodeStruct *current = head;
	while (current != NULL) {
		if (current->to_index == to_index) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

struct nodeStruct* List_findFirstNode(struct nodeStruct *head, int size){
	struct nodeStruct *current = head;
	while (current != NULL){
		if (current->size >= size){
			return current;
		}
		current = current->next;
	}
	return NULL;
}

struct nodeStruct* List_findBiggestNode(struct nodeStruct *head, int size){
	struct nodeStruct *biggest = NULL;
	struct nodeStruct *current = head;
	while ((current != NULL) && (current->size < size)) {
		current = current->next;
	}
	biggest = current;
	while (current != NULL) {
		if ((biggest != NULL) && (current->size >= size) && (current->size > biggest->size)){
			biggest = current;
		}
		current = current->next;
	}
	return biggest;
}

struct nodeStruct* List_findSmallestNode(struct nodeStruct *head, int size){
	struct nodeStruct *smallest = NULL;
	struct nodeStruct *current = head;
	while ((current != NULL) && (current->size < size)) {
		current = current->next;
	}
	smallest = current;
	while (current != NULL){
		if ((smallest != NULL) && (current->size >= size) && (current->size < smallest->size)) {
			smallest = current;
		}
		current = current->next;
	}
	return smallest;
}

void List_modifyNode(struct nodeStruct* node, int size, int from_index, int to_index){
	node->size = size;
	node->from_index = from_index;
	node->to_index = to_index;
}

/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node)
{
	assert(headRef != NULL);
	assert(*headRef != NULL);

	// Is it the first element?
	if (*headRef == node) {
		*headRef = node->next;
	}
	else {
		// Find the previous node:
		struct nodeStruct *previous = *headRef;
		while (previous->next != node) {
			previous = previous->next;
			assert(previous != NULL);
		}

		// Unlink node:
		assert(previous->next == node);
		previous->next = node->next;
	}

	// Free memory:
	free(node);
}


/*
 * Sort the list in ascending order based on the from_index field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef)
{
	while (doSinglePassOnSort(headRef)) {
		// Do nothing: work done in loop condition.
	}
}
static _Bool doSinglePassOnSort(struct nodeStruct **headRef)
{
	_Bool didSwap = false;
	while (*headRef != NULL) {
		struct nodeStruct *nodeA = *headRef;
		// If we don't have 2 remaining elements, nothing to swap.
		if (nodeA->next == NULL) {
			break;
		}
		struct nodeStruct *nodeB = nodeA->next;

		// Swap needed?
		if (nodeA->from_index > nodeB->from_index){
			swapElements(headRef, nodeA, nodeB);
			didSwap = true;
		}

		// Advance to next elements
		headRef = &((*headRef)->next);
	}
	return didSwap;
}
static void swapElements(struct nodeStruct **previous,
		struct nodeStruct *nodeA,
		struct nodeStruct *nodeB)
{
	*previous = nodeB;
	nodeA->next = nodeB->next;
	nodeB->next = nodeA;
}

void List_print (struct nodeStruct *headRef){
	struct nodeStruct* current = headRef;
	while (current != NULL){
		printf ("{%d. %d-%d}, ", current->size,current->from_index, current->to_index);
		current = current->next;
	}
	printf ("\n");
}