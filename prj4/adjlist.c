#include "adjlist.h"
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>


/*
 * Allocate memory for a node of type struct ListNode and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct ListNode* List_createNode(int item)
{
	struct ListNode *pNode = malloc(sizeof(struct ListNode));
	if (pNode != NULL) {
		pNode->item = item;
	}
	return pNode;
}

struct Vertex* List_createVertex(int item, int is_thread, pthread_t threadID)
{
	struct Vertex *pNode = malloc(sizeof(struct Vertex));
	if (pNode != NULL) {
			pNode->item = item;
		if (is_thread == 1){
			pNode->is_thread_node = 1;
			pNode->PID = threadID;
		}
		else {
			pNode->is_thread_node = 0;
			pNode->PID = threadID;
		}
		pNode->head = NULL;
	}

	return pNode;
}

/*
 * Insert node after the tail of the list.
 */
void List_insertNode (struct ListNode **headRef, struct ListNode *node)
{
	node->next = NULL;

	// Handle empty list
	if (*headRef == NULL) {
		*headRef = node;
	}
	else {
		// Find the tail and insert node
		struct ListNode *current = *headRef;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = node;
	}
}

void List_insertVertex (struct Vertex **headRef, struct Vertex *node)
{
	node->next = NULL;

	// Handle empty list
	if (*headRef == NULL) {
		*headRef = node;
	}
	else {
		// Find the tail and insert node
		struct Vertex *current = *headRef;
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
int List_countNodes (struct ListNode *head)
{
	int count = 0;
	struct ListNode *current = head;
	while (current != NULL) {
		current = current->next;
		count++;
	}
	return count;
}

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct ListNode* List_findNode(struct ListNode *head, int item)
{
	struct ListNode *current = head;
	while (current != NULL) {
		if (current->item == item) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

struct Vertex* List_findVertex(struct Vertex* head, int is_thread, int item, pthread_t threadID)
{
	struct Vertex *current = head;
	if (is_thread == 0){
		while (current != NULL) {
			if (current->item == item) {
				return current;
			}
			current = current->next;
		}
	}
	if (is_thread == 1){
		while (current != NULL) {
			if ((current->is_thread_node == 1) && (pthread_equal(threadID, current->PID))) {
				return current;
			}
			current = current->next;
		}
	}
	return NULL;
}
/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */
void List_deleteNode (struct ListNode **headRef, struct ListNode *node)
{
	assert(headRef != NULL);
	assert(*headRef != NULL);

	// Is it the first element?
	if (*headRef == node) {
		*headRef = node->next;
	}
	else {
		// Find the previous node:
		struct ListNode *previous = *headRef;
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

