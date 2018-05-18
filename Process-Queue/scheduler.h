#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process/utils.h"
#include "process/process.h"
#include "FIFORDRN_Scheduler.h"
#include "MLFQ_Scheduler.h"

using namespace std;

class Scheduler {
	public: 
		Scheduler(int CPUs, string pol, int timeslice_arg); /* constructor */
		~Scheduler(); /* deconstructor */
		int runningsize(); /* get size of running array */
		void add_task(string cmd); /* add task for server */
		string process_command(string client_message); /* process the command sent by client */
		string print_waiting(); /* print waiting processes */
		int waitingsize(); /* get number of waiting processes */
		void iterate_running_processes(); /* iterate through running processes to preempt and start new process */
		string print_running(); /* print running processes */
		string print_status(); /* print status of running and waiting processes */
		string flush(); /* clear running and waiting processes */

	private:
		int ncpus;
		string policy;
		int timeslice;
		process* running; /* array of running processes */
		int nprocessesrun;
		int nprocessesadded;

		/* Schedulers */
		FIFORDRN_Scheduler fr_sched;
		MLFQ_Scheduler MLFQ_sched; 

		time_t totalTurnaround;
		time_t totalResponse;
		time_t avgturn;
		time_t avgresp;
		int boost_time;

};
	
#endif
