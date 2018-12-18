#include "stats.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void stats_init(int num_producers){
	headfactory = malloc(num_producers*(sizeof(struct producer)));
	for( int i = 0; i < num_producers; i++){
		headfactory[i].factory_number = i;
		headfactory[i].candies_made = 0;
		headfactory[i].candies_consumed = 0;
		headfactory[i].total_delay = 0;
		headfactory[i].min_delay = 0;
		headfactory[i].avg_delay = 0;
		headfactory[i].max_delay = 0;
	}
	numprod = num_producers;
}


void stats_cleanup(void){
		free(headfactory);
}


void stats_record_produced(int factory_number){
	headfactory[factory_number].candies_made++;
}


void stats_record_consumed(int factory_number, double delay_in_ms){
	//Updating min and max delay
	if (headfactory[factory_number].candies_consumed == 0){
		headfactory[factory_number].min_delay = delay_in_ms;
		headfactory[factory_number].max_delay = delay_in_ms;
	}
	else{
		if (delay_in_ms < headfactory[factory_number].min_delay){
			headfactory[factory_number].min_delay = delay_in_ms;
		}
		if (delay_in_ms > headfactory[factory_number].max_delay){
			headfactory[factory_number].max_delay = delay_in_ms;
		}
	}
	//Calculating avg delay
	headfactory[factory_number].candies_consumed++;
	headfactory[factory_number].max_delay += delay_in_ms;
	headfactory[factory_number].avg_delay = 
	(headfactory[factory_number].max_delay) / headfactory[factory_number].candies_consumed;
}

void stats_display(void){
	_Bool error = false;
	printf("%8s%10s%10s%15s%15s%15s\n",
		"Factory#", 
		"#Made", 
		"#Eaten", 
		"Min Delay[ms]",
		"Avg Delay[ms]", 
		"Max Delay[ms]");
	for (int i = 0; i <numprod; i++){
		printf("%8d%10d%10d%15.5f%15.5f%15.5f\n", 
		headfactory[i].factory_number,
		headfactory[i].candies_made,
		headfactory[i].candies_consumed,
		headfactory[i].min_delay,
		headfactory[i].avg_delay,
		headfactory[i].max_delay);
		if (headfactory[i].candies_consumed !=
			headfactory[i].candies_made){error = true;}
	}
	if (error == true){
		printf("ERROR: Mismatch between number made and eaten.\n");
	}

}