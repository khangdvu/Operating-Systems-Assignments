//Linked list module provided by Professor Keval Vora of SFU, modified by Khang Vu.

#ifndef LIST_H_
#define LIST_H_

struct nodeStruct {
    int size;			//Item is size of memory chunk;
    int from_index;
    int to_index;
    struct nodeStruct *next;
};

/*
 * Allocate memory for a node of type struct nodeStruct and initialize
 * it with the value size. Return a pointer to the new node.
 */
struct nodeStruct* List_createNode(int size, int from_index, int to_index);

/*
 * Insert node at the head of the list.
 */
void List_insertHead (struct nodeStruct **headRef, struct nodeStruct *node);

/*
 * Insert node after the tail of the list.
 */
void List_insertTail (struct nodeStruct **headRef, struct nodeStruct *node);

/*
 * Count number of nodes in the list.
 * Return 0 if the list is empty, i.e., head == NULL
 */
int List_countNodes (struct nodeStruct *head);

/*
 * Return the first node holding the value size, return NULL if none found
 */
struct nodeStruct* List_findNodebySize(struct nodeStruct *head, int size);

struct nodeStruct* List_findNodeby_fromIndex(struct nodeStruct *head, int from_index);

struct nodeStruct* List_findNodeby_toIndex(struct nodeStruct *head, int to_index);

struct nodeStruct* List_findFirstNode(struct nodeStruct *head, int size);

struct nodeStruct* List_findBiggestNode(struct nodeStruct *head, int size);

struct nodeStruct* List_findSmallestNode(struct nodeStruct *head, int size);

void List_modifyNode(struct nodeStruct* node, int size, int from_index, int to_index);
/*
 * Delete node from the list and free memory allocated to it.
 * This function assumes that node has been properly set (by for example
 * calling List_findNode()) to a valid node in the list. If the list contains
 * only node, the head of the list should be set to NULL.
 */
void List_deleteNode (struct nodeStruct **headRef, struct nodeStruct *node);

/*
 * Sort the list in ascending order based on the from_index field.
 * Any sorting algorithm is fine.
 */
void List_sort (struct nodeStruct **headRef);

void List_print (struct nodeStruct *headRef);

#endif
