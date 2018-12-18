#include "klock.h"
#include "adjlist.h"
#include <stdio.h>

int initialized = 0;
int nodecounter = 0; 
struct Vertex* headVertex;
struct ListNode* headNode;

void init_lock(SmartLock* lock) {
	if (initialized == 0){
		headVertex = List_createVertex(nodecounter, 0, pthread_self());
		headNode = List_createNode(nodecounter);
		initialized = 1;
	}
	else{
		lock->item = nodecounter;
		struct Vertex* newLock = List_createVertex(nodecounter, 0, pthread_self());
		List_insertVertex(&(headVertex), newLock);
	}
	lock->item = nodecounter;
	pthread_mutex_init(&(lock->mutex), NULL);
	nodecounter ++;
}

int lock(SmartLock* lock) {
	struct Vertex* currentThread = List_findVertex(headVertex, 1, 0, pthread_self());
	struct Vertex* currentLock = List_findVertex(headVertex, 0 , lock->item, pthread_self());

	if (currentThread == NULL){
		currentThread = List_createVertex(nodecounter, 1, pthread_self());
		nodecounter++;
	}

	if (currentLock != NULL){
		printf("Lock %d and Thread %d\n", currentLock->item, currentThread->item);
	}
	struct ListNode* newListNode = List_findNode(currentLock->head, currentThread->item);
	if (newListNode == NULL){
		newListNode = List_createNode(currentThread->item);
		List_insertNode(&(currentLock->head), newListNode);
	}
	printf("%lu locking\n", pthread_self());
	pthread_mutex_lock(&(lock->mutex));
	return 1;
}

void unlock(SmartLock* lock) {

	pthread_mutex_unlock(&(lock->mutex));
}

/*
 * Cleanup any dynamic allocated memory for SmartLock to avoid memory leak
 * You can assume that cleanup will always be the last function call
 * in main function of the test cases.
 */
void cleanup() {

}
