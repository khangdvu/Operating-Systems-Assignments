//Code provided by Professor Keval Vora for use. Modified by Khang Vu

#ifndef ADJLIST_H_
#define ADJLIST_H_
#include <pthread.h>
struct ListNode {
    int item;
    struct ListNode *next;
};

struct Vertex {
	int item;
	int is_thread_node;
	pthread_t PID;
	struct ListNode *head;
    struct Vertex* next;
};

/*
 * Allocate memory for a node of type struct ListNode and initialize
 * it with the value item. Return a pointer to the new node.
 */
struct ListNode* List_createNode(int item);
struct Vertex* List_createVertex(int item, int is_thread, pthread_t threadID);

/*
 * Insert node after the tail of the list.
 */
void List_insertNode (struct ListNode **headRef, struct ListNode *node);
void List_insertVertex (struct Vertex **headRef, struct Vertex *node);

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */

int List_countNodes (struct ListNode *head);

/*
 * Return the first node holding the value item, return NULL if none found
 */
struct ListNode* List_findNode(struct ListNode *head, int item);
struct Vertex* List_findVertex(struct Vertex* head, int is_thread, int item, pthread_t threadID);
/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */
void List_deleteNode (struct ListNode **headRef, struct ListNode *node);



#endif
