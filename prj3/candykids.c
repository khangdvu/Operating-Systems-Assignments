#include "bbuff.h"
#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

_Bool stop_thread = false;

// semaphore declarations
sem_t mutex;
sem_t buffer_full;
sem_t buffer_empty;


typedef struct {
    int factory_number;
    double time_stamp_in_ms;
} candy_t;


double current_time_in_ms(void){
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

void* factory_thread(void* fnum){
	while (!stop_thread){

		int waittime = rand() % 3;
		printf("\tFactory %d ships candy & waits %ds\n", *(int*)fnum, waittime);

		sem_wait(&buffer_empty);
		sem_wait(&mutex);

		candy_t *candy = malloc(sizeof(candy_t));
	    candy->factory_number = *(int*)fnum;
	    candy->time_stamp_in_ms = current_time_in_ms();
	    bbuff_blocking_insert(candy);
	    stats_record_produced(*(int*)fnum);

	    sem_post(&mutex); 
	   	sem_post(&buffer_full);

	   	sleep(waittime);
	}
	printf("Candy-factory %d done\n", *(int*)fnum);
	return NULL;
}

void* kid_thread(void* knum){
	while (true){

		sem_wait(&buffer_full);
		sem_wait(&mutex);

		candy_t* candy = bbuff_blocking_extract();
		stats_record_consumed(candy->factory_number, (current_time_in_ms() - candy->time_stamp_in_ms));
		free(candy);

		sem_post(&mutex);
		sem_post(&buffer_empty);

		sleep(rand() % 1);
	}
	return NULL;
}


int main(int argc, char *argv[]){

	if (argc != 4) {printf("Invalid number of arguments. Exiting \n"); return -1;}
	int factories_to_spawn = atoi(argv[1]);
	int kids_to_spawn = atoi(argv[2]);
	int seconds_to_run = atoi(argv[3]);

	if (factories_to_spawn <= 0 || kids_to_spawn <= 0 || seconds_to_run <= 0){
		printf("One or more arguments are invalid. Exiting.\n"); return -1;
	}

	//Module Initializations
	bbuff_init();
	stats_init(factories_to_spawn);

	//Semaphore Initializations
	sem_init(&mutex, 0, 1);
	sem_init(&buffer_full, 0, 0);
	sem_init(&buffer_empty, 0, BUFFER_SIZE);

	//////////////
	//Spawning Factory Threads
	/////////////
	pthread_t factories[factories_to_spawn];
	int indexes1[factories_to_spawn];

	for (int i =0; i < factories_to_spawn; i++){
		indexes1[i] = i;
		pthread_create(&factories[i], NULL, factory_thread, (void *) &indexes1[i]);
	}


	//////////////
	//Spawning Kid Threads
	/////////////
	pthread_t kids[kids_to_spawn];
	int indexes2[kids_to_spawn];

	for (int i =0; i < kids_to_spawn; i++){
		indexes2[i] = i;
		pthread_create(&kids[i], NULL, kid_thread, (void *) &indexes2[i]);
	}

	for (int t = 0; t < seconds_to_run; t++){
		sleep(1);
		printf("Time %ds\n", t);
	}
	sleep(1);
	
	//Signal for Factory threads to stop
	stop_thread = true;

	for (int i =0; i < factories_to_spawn; i++){
		pthread_join(factories[i], NULL);
	}

	//Waiting all candies to be consumed and stop Kid threads
	while (bbuff_is_empty() == false){
		printf("Waiting for all candy to be consumed\n");
		sleep(1);
	}

	printf("Stopping kids.\n");
	for (int i =0; i < kids_to_spawn; i++){
		pthread_cancel(kids[i]);
 		pthread_join(kids[i], NULL);
	}


	stats_display();
	stats_cleanup();
	return 0;
}