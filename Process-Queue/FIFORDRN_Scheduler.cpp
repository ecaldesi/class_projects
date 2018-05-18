#include "process/utils.h"
#include "process/process.h"
#include <queue> 
#include "FIFORDRN_Scheduler.h"

using namespace std;

/* constructor */
FIFORDRN_Scheduler::FIFORDRN_Scheduler() {
	size = 0;
}

/* get number of waiting processes */
int FIFORDRN_Scheduler::waiting_size() {
	return waiting.size();
}

/* print waiting processes */
string FIFORDRN_Scheduler::print_waiting() {
	string tmp_str;
	tmp_str = print_header();
	queue <process> tmp = waiting;
	process p("",0);
	while (!tmp.empty() ) {
		p = tmp.front();
		tmp.pop();
		tmp_str = tmp_str + p.format_print();
	}
	return tmp_str;
}

/* flush waiting processes */
void FIFORDRN_Scheduler::flush() {
	process p;
	while (!waiting.empty()) {
		p = waiting.front();
		if (p.PID != 0){
			p.end();
		}
		waiting.pop();
	}
}

/* take the next process from waiting queue */
process FIFORDRN_Scheduler::next_in_waiting() {
	process p;
	if (!waiting.empty()) {
		p = waiting.front();
		waiting.pop();
	} 

	return p;
}

/* add process to waiting queue */
void FIFORDRN_Scheduler::add_to_waiting(process p) {
	waiting.push(p);
}
