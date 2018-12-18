#include <stdio.h>
#include "kallocator.h"

int main(int argc, char* argv[]) {
    initialize_allocator(100, FIRST_FIT);
    //initialize_allocator(100, BEST_FIT);
    //initialize_allocator(100, WORST_FIT);
    printf("Using first fit algorithm on memory size 100\n");

    int* p[50] = {NULL};
    for(int i=0; i<10; ++i) {
        printf("Allocating size %ld\n", sizeof(int));
        p[i] = kalloc(sizeof(int));

        if(p[i] == NULL) {
            printf("Allocation failed\n\n");
            continue;
        }
        *(p[i]) = i;
        printf("p[%d] = %p ; *p[%d] = %d\n\n", i, p[i], i, *(p[i]));
    }

    print_statistics();

    for(int i=0; i<10; ++i) {
/*        if(i%2 == 0)
            continue;*/
        if(i%2 == 0){
        printf("Freeing p[%d]\n", i);
        kfree(p[i]);
        p[i] = NULL;
        }
    }

/*    printf("SECOND RUN\n");


    for(int i=1; i<15; ++i) {
        printf("Allocating size %d\n", i);
        p[i] = kalloc(i);
        if(p[i] == NULL) {
                printf("Allocation failed\n\n");
                continue;
            }
            printf("p[%d] = %p;\n\n", i, p[i]);
        }

for(int i=1; i<4; ++i) {
        printf("Allocating size %d\n", i);
        p[i] = kalloc(i);
        if(p[i] == NULL) {
                printf("Allocation failed\n\n");
                continue;
            }
            printf("p[%d] = %p;\n\n", i, p[i]);
        }
        print_statistics();

*/

    printf("available_memory %d\n", available_memory());

    void* before[100] = {NULL};
    void* after[100] = {NULL};
    int yolo = compact_allocation(before, after);
    printf("POINTERS FROM\tTO-:\n");
    for (int i = 0; i < yolo; i++){
        printf("%p \t %p\n", before[i], after[i] );
    }

    print_statistics();

    // You can assume that the destroy_allocator will always be the 
    // last funciton call of main function to avoid memory leak 
    // before exit

    destroy_allocator();

    return 0;
}
