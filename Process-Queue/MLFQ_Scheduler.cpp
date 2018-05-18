#include "MLFQ_Scheduler.h"
#include "process/utils.h"
#include <string>
/* return number of waiting processes */
int MLFQ_Scheduler::waiting_size() {
	int size = 0;
	for (int i = 0; i< 5; i++) {
		size = size + waiting[i].size();
	}
	return size;
}

/* print waiting queues */
string MLFQ_Scheduler::print_waiting() {
	process p;
	queue <process> tmp;
	string tmp_str = "";

	for (int i =0; i< 5; i++) {
		tmp = waiting[i];
		if(!tmp.empty()){
			tmp_str = tmp_str + "Waiting Queue Level: " + to_string(i) + "\n";
			tmp_str = tmp_str + print_header();
			while (!tmp.empty()) {
				p = tmp.front();
				tmp_str = tmp_str + p.format_print(); /* print process */
				tmp.pop();
			}
			tmp_str = tmp_str + "\n";
		}
	}
	return tmp_str;
}
 /* clear waiting processes */
void MLFQ_Scheduler::flush() {
	process p;
	for (int i = 0; i< 5; i++) {
		while (!waiting[i].empty()) {
			p = waiting[i].front();
			p.end();
			waiting[i].pop();
		}
	}
}

/* get next process waiting */
process MLFQ_Scheduler::next_in_waiting() {
	process p;
	for (int i =0; i< 5; i++) {
		if (!waiting[i].empty()) {
			p = waiting[i].front(); /* find first process waiting */
			waiting[i].pop();
			break;
		}
	}	
	return p;	
}

/* add process to waiting */
void MLFQ_Scheduler::add_to_waiting(process p) {
	/* if process has never been run */
	if (p.PID == 0) {
		waiting[0].push(p); /* add it to the highest priority level */
	} else {
		/* if process has only run once */
		if (priority.find(p.PID) == priority.end()) {
			priority[p.PID] = 1; /* set its priority to 1 */
			waiting[1].push(p);
		} else {
		/* process has a priority, lower it by 1 */
			if(priority[p.PID] < 4){
				priority[p.PID] = priority[p.PID] + 1;
			} 
			waiting[priority[p.PID]].push(p);
		} 
	}
}

/* boost all waiting processes */
void MLFQ_Scheduler::boost() {
	/* set priority to 0 */
	for (auto it = priority.begin(); it != priority.end(); it++) {
		it->second = 0;
	}
	process p;
	for (int i =1; i< 5; i++) {
		while (!waiting[i].empty()) {
			p = waiting[i].front();
			waiting[0].push(p); /* push all process to priority 0 */
			waiting[i].pop();
		}			
	}
}
