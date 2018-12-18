#include "list.h"
#include <stdio.h>
#include <stdlib.h>


/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int item){
	struct nodeStruct* node = malloc(sizeof(struct nodeStruct));
	node->item = item;
	node->next = NULL;
	return node;
}


/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node){
	node->next = *headRef;
	*headRef = node;
	return;
}


/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct* iterate = *headRef;
	while (iterate->next != NULL){
		iterate = iterate->next;
	}
	iterate->next = node;
	node->next = NULL;
	return;

}


/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head){
	struct nodeStruct* node = head;
	int count = 0;
	while (node != NULL){
		count++;
		node = node->next;
	}
	return count;
}


/*
 * Return the first node holding the value item, return NULL if none found
 */
struct nodeStruct* List_findNode(struct nodeStruct *head, int item){
	struct nodeStruct* node = head;
	while (node != NULL){
		if (node->item == item){
			return node;
		}
		else{
			node = node->next;
		}
	}
	return node;
}


/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set to a valid node 
 * in the list. For example, the client code may have found it by calling 
 * List_findNode(). If the list contains only one node, the head of the list 
 * should be set to NULL.
 */
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node){
	struct nodeStruct* iterate = *headRef;

	while (iterate->next != node){
		iterate = iterate->next;
	}

	iterate->next = iterate->next->next;
	free(node);

	if (List_countNodes(*headRef) == 0) {*headRef = NULL;}
	return;
}


/*
 * Sort the list in ascending order based on the item field.
 * Any sorting algorithm is fine.
 *
 * Insertion sort *
 */
void List_sort (struct nodeStruct **headRef){
	struct nodeStruct* inode = *headRef;
	struct nodeStruct* jnode = *headRef;
	struct nodeStruct* minnode = *headRef;

	int swap = 0;

	while (inode != NULL){
		jnode = inode->next;
		minnode = inode;
		while (jnode != NULL){
			if (jnode->item < minnode->item){
				minnode = jnode;
			}
			jnode = jnode->next;
		}
		swap = inode->item;
		inode->item = minnode->item;
		minnode->item = swap;
		inode = inode->next;
	}
	return;
}
