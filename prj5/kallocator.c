#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "kallocator.h"
#include "list.h"

struct KAllocator {
    enum allocation_algorithm aalgorithm;
    int size;
    void* memory;
    struct nodeStruct *allocated_nodes;
    struct nodeStruct *free_nodes;
};

struct KAllocator kallocator;

                                    //enum allocation_algorithm {FIRST_FIT, BEST_FIT, WORST_FIT};
void initialize_allocator(int _size, enum allocation_algorithm _aalgorithm) {
    assert(_size > 0);
    kallocator.aalgorithm = _aalgorithm; 
    kallocator.size = _size;
    kallocator.memory = malloc((size_t)kallocator.size);
    kallocator.allocated_nodes = NULL;
    kallocator.free_nodes = List_createNode(_size, 0, _size-1);
}

void destroy_allocator() {
    free(kallocator.memory);

    struct nodeStruct *current;
    struct nodeStruct *temp;

    //Free allocated chunks and free chunks lists
    current = kallocator.allocated_nodes;
    while (current != NULL){
        temp = current;
        current = current->next;
        free(temp);
    }
    current = kallocator.free_nodes;
    while (current != NULL){
        temp = current;
        current = current->next;
        free(temp);
    }
}


void* kalloc(int _size) {
    void* ptr = NULL;
    //If size is invaLid, return NULL pointer;
    if (_size < 0){return ptr;}
    struct nodeStruct* newNode;
    struct nodeStruct* jNode;




    //Create chunk to be returned when no memory is yet allocated
    if (kallocator.allocated_nodes == NULL){
        newNode = List_createNode(_size, 0, _size-1);                                 
        List_insertHead (&(kallocator.allocated_nodes), newNode);
        jNode = List_findNodeby_fromIndex((kallocator.free_nodes), 0);
        int size = jNode->size - _size;
        int from_index = jNode->from_index + _size;
        int to_index = jNode->to_index;
        List_modifyNode(jNode, size, from_index, to_index);

        ptr = (kallocator.memory+0);
    }

    //ELSE Create chunk to be returned with:

    //First Fit Algorithm
    else if (kallocator.aalgorithm == 0){
        jNode = List_findFirstNode(kallocator.free_nodes, _size);
        if (jNode != NULL){
            newNode = List_createNode(_size, jNode->from_index, jNode->from_index + _size - 1);
            List_insertTail(&(kallocator.allocated_nodes), newNode);
            int size = jNode->size - _size;
            int from_index = jNode->from_index + _size;
            int to_index = jNode->to_index;
            if (size == 0){
                List_deleteNode (&(kallocator.free_nodes), jNode);
            }
            else{
                List_modifyNode(jNode, size, from_index, to_index);
            }
            ptr = (kallocator.memory + (newNode->from_index));
        }
    }

    //Best Fit Algorithm
    else if (kallocator.aalgorithm == 1){
        jNode = List_findSmallestNode(kallocator.free_nodes, _size);
        if (jNode != NULL){
            newNode = List_createNode(_size, jNode->from_index, jNode->from_index + _size - 1);
            List_insertTail(&(kallocator.allocated_nodes), newNode);
            int size = jNode->size - _size;
            int from_index = jNode->from_index + _size;
            int to_index = jNode->to_index;
            if (size == 0){
                List_deleteNode (&(kallocator.free_nodes), jNode);
            }
            else{
                List_modifyNode(jNode, size, from_index, to_index);
            }
            ptr = (kallocator.memory + (newNode->from_index));
        }
    }

    //Worst Fit Algorithm
 else if (kallocator.aalgorithm == 2){
        jNode = List_findBiggestNode(kallocator.free_nodes, _size);
        if (jNode != NULL){
            newNode = List_createNode(_size, jNode->from_index, jNode->from_index + _size - 1);
            List_insertTail(&(kallocator.allocated_nodes), newNode);
            int size = jNode->size - _size;
            int from_index = jNode->from_index + _size;
            int to_index = jNode->to_index;
            if (size == 0){
                List_deleteNode (&(kallocator.free_nodes), jNode);
            }
            else{
                List_modifyNode(jNode, size, from_index, to_index);
            }
            ptr = (kallocator.memory + (newNode->from_index));
        }
    }

    //Printing out lists
    List_sort (&(kallocator.allocated_nodes));
    List_sort (&(kallocator.free_nodes));
/*    printf("Allocated Chunks:\n");
    List_print(kallocator.allocated_nodes);
    printf("Free Chunks:\n");
    List_print(kallocator.free_nodes);*/

    return ptr;
}

void kfree(void* _ptr) {
    assert(_ptr != NULL);
    struct nodeStruct* alloChunk;
    struct nodeStruct* freebeforeChunk;
    struct nodeStruct* freeafterChunk;

    //Search and obtain the allocated chunk, 
    //and possible free chunks before or after it to be merged later
    for (int i = 0; i < kallocator.size; i++){
        if (kallocator.memory + i == _ptr){
            alloChunk = List_findNodeby_fromIndex(kallocator.allocated_nodes, i);
            freebeforeChunk = List_findNodeby_toIndex(kallocator.free_nodes, alloChunk->from_index - 1);
            freeafterChunk = List_findNodeby_fromIndex(kallocator.free_nodes, alloChunk->to_index + 1);
        }
    }
    int size;
    int from_index;
    int to_index;

    //Case 1: There are free chunks before and after the node to be freed
    if ((freebeforeChunk != NULL) && (freeafterChunk != NULL)){
        size = freebeforeChunk->size + alloChunk->size + freeafterChunk->size;
        from_index = freebeforeChunk->from_index;
        to_index = freeafterChunk->to_index;
        List_modifyNode(freebeforeChunk, size, from_index, to_index );
        List_deleteNode (&(kallocator.allocated_nodes), alloChunk);
        List_deleteNode(&(kallocator.free_nodes), freeafterChunk);
    }

    //Case 2: There is only a free chunk before the node to be freed
    else if (freebeforeChunk != NULL){
        size = freebeforeChunk->size + alloChunk->size;
        from_index = freebeforeChunk->from_index;
        to_index = alloChunk->to_index;
        List_modifyNode(freebeforeChunk, size, from_index, to_index );
        List_deleteNode (&(kallocator.allocated_nodes), alloChunk);
    }

    //Case 3: There is only a free chunk after the node to be freed
    else if (freeafterChunk != NULL){
        size = alloChunk->size + freeafterChunk->size;
        from_index = alloChunk->from_index;
        to_index = freeafterChunk->to_index;
        List_modifyNode(freebeforeChunk, size, from_index, to_index );
        List_deleteNode (&(kallocator.allocated_nodes), alloChunk);
    }

    //Case 4: There are no free chunks before nor after the node to be freed
    else {
        size = alloChunk->size;
        from_index = alloChunk->from_index;
        to_index = alloChunk->to_index;
        struct nodeStruct* newNode = List_createNode(size, from_index, to_index);
        List_insertTail(&(kallocator.free_nodes), newNode);
        List_deleteNode(&(kallocator.allocated_nodes), alloChunk);
    }

    List_sort (&(kallocator.allocated_nodes));
    List_sort (&(kallocator.free_nodes));
    //Printing Module
/*    printf("Allocated Chunks:\n");
    List_print(kallocator.allocated_nodes);
    printf("Free Chunks:\n");
    List_print(kallocator.free_nodes);
    printf("\n");  */
}


int compact_allocation(void** _before, void** _after) {
    int compacted_size = 0;

    int current_start = 0;
    int node_start = 0;
    struct nodeStruct* jNode = kallocator.allocated_nodes;
    int last;
    if (jNode->from_index == 0){
        current_start = jNode->to_index + 1;
        jNode = jNode->next;
    }

    while(jNode != NULL){
        if (jNode->from_index != current_start){
            node_start = jNode->from_index;
            jNode->from_index = current_start;
            jNode->to_index = jNode->from_index + (jNode->size - 1);

            _before[compacted_size] = kallocator.memory + node_start;
            _after[compacted_size] = kallocator.memory + current_start;
            compacted_size++;
        }
        current_start = jNode->to_index + 1;
        last = jNode->to_index;
        jNode = jNode->next;
    }

    struct nodeStruct* temp;

    jNode = kallocator.free_nodes;
    while (jNode->next != NULL){
        temp = jNode;
        jNode = jNode->next;
        List_deleteNode(&(kallocator.free_nodes), temp);
        }
    jNode->from_index = last +1;
    jNode->to_index = kallocator.size - 1;
    jNode->size = jNode->to_index - jNode->from_index + 1; 
/*    printf("AFTER COMPACTION\n");
    printf("Allocated Chunks:\n");
    List_print(kallocator.allocated_nodes);
    printf("Free Chunks:\n");
    List_print(kallocator.free_nodes);
    printf("\n");  */
    return compacted_size;
}

int available_memory() {
    int available_memory_size = 0;
    struct nodeStruct* current = kallocator.free_nodes;
    while (current != NULL){
        available_memory_size = available_memory_size + current->size;
        current = current->next;
    }
    return available_memory_size;
}

void print_statistics() {
    int allocated_size = 0;
    int allocated_chunks = 0;
    int free_size = 0;
    int free_chunks = 0;
    int smallest_free_chunk_size = kallocator.size;
    int largest_free_chunk_size = 0;

    // Calculate the statistics
    struct nodeStruct* current;

    current =  kallocator.allocated_nodes;
    while (current != NULL){
        allocated_size = allocated_size + current->size;
        current = current->next;
    }

    current =  kallocator.allocated_nodes;
    while (current != NULL){
        allocated_chunks++;
        current = current->next;
    }

    free_size = available_memory();

    current =  kallocator.free_nodes;
    while (current != NULL){
        free_chunks++;
        current = current->next;
    }

    current = List_findSmallestNode(kallocator.free_nodes, 1);
    smallest_free_chunk_size = current->size;

    current = List_findBiggestNode(kallocator.free_nodes, 1);
    largest_free_chunk_size = current->size;

    printf("Allocated size = %d\n", allocated_size);
    printf("Allocated chunks = %d\n", allocated_chunks);
    printf("Free size = %d\n", free_size);
    printf("Free chunks = %d\n", free_chunks);
    printf("Largest free chunk size = %d\n", largest_free_chunk_size);
    printf("Smallest free chunk size = %d\n", smallest_free_chunk_size);
}



