#ifndef STATS_H
#define STATS_H


struct producer{
	int factory_number;
	int candies_made;
	int candies_consumed;
	double total_delay;
	double min_delay;
	double avg_delay;
	double max_delay;
};

struct producer* headfactory;
int numprod;

void stats_init(int num_producers);
void stats_cleanup(void);
void stats_record_produced(int factory_number);
void stats_record_consumed(int factory_number, double delay_in_ms);
void stats_display(void);

#endif