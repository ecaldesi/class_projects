#ifndef FIFORDRN_SCHEDULER_H
#define FIFORDRN_SCHEDULER_H

#include "process/utils.h"
#include "process/process.h"
#include <queue> 

using namespace std;

class FIFORDRN_Scheduler {
	public:
		FIFORDRN_Scheduler(); /*constructor */
		int waiting_size(); /* get size of waiting queue */
		string print_waiting(); /* print waiting queue */
		void flush(); /* clear waiting processes */
		process next_in_waiting(); /* get the next process from waiting queue */
		void add_to_waiting(process p); /* add process to waiting queue */
	private:
		queue <process> waiting;
		int size;
};

#endif
