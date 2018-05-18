#ifndef MLFQ_SCHEDULER_H
#define MLFQ_SCHEDULER_H

#include "process/utils.h"
#include "process/process.h"
#include <queue> 
#include <map>

class MLFQ_Scheduler {
	public:
		void setsize(); /* set number of waiting processes */
		int waiting_size(); /* get number of waiting processes */
		string print_waiting(); /* print waiting processes */
		void flush(); /* clear all waiting processes */
		process next_in_waiting(); /* get the next waiting process */
		void add_to_waiting(process p); /* add process to waiting processes */
		void boost(); /* boost all priorities */	

	private:
		queue <process> waiting[5];
		map <pid_t, int> priority;
};

#endif
