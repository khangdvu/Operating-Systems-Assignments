#include "bbuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void bbuff_init(void){
	number_of_candy = 0;
}

void bbuff_blocking_insert(void* item){
	if (number_of_candy >= (BUFFER_SIZE)){printf("Error inserting.\n"); return;}
	candy_buffer[number_of_candy] = item;
	number_of_candy++;
	return;
}

void* bbuff_blocking_extract(void){
	if (number_of_candy == 0)	{printf("Error extracting.\n"); return NULL;}
	void *extracted = candy_buffer[0];
	for (int i = 0; i < (number_of_candy-1); i++){
		candy_buffer[i] = candy_buffer[i+1];
	}
	number_of_candy--;
	return extracted;
}

_Bool bbuff_is_empty(void){
	if (number_of_candy == 0){return true;}
	return false;
}
