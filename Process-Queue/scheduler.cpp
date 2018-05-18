#include "process/utils.h"
#include "process/process.h"
#include "scheduler.h"
#include "FIFORDRN_Scheduler.h"
#include "MLFQ_Scheduler.h"
#include <csignal>
#include <cstring>
#include <string>
#include <sstream>

using namespace std;

/* constructor */
Scheduler::Scheduler(int CPUs, string pol, int timeslice_arg){

	ncpus = CPUs;
	policy = pol;
	timeslice = timeslice_arg;
	running = new process[ncpus]; 
	nprocessesrun = 0;
	nprocessesadded = 0;
	totalTurnaround = 0;
	totalResponse = 0;
	avgturn = 0;
	avgresp = 0;
	boost_time = 40;

}

/* deconstructor */
Scheduler::~Scheduler() {
	delete [] running;
}

/* find number of processes being run at a certain time */
int Scheduler::runningsize() {
	int rsize = 0;
	for (int i = 0; i < ncpus; i++) {
		if (running[i].PID != -1) {
			rsize = rsize + 1;
		} 
	}
	return rsize;
}

/* get number of waiting processes */
int Scheduler::waitingsize() {
	int wsize = 0;
	if (policy == "fifo" || policy == "rdrn") {
		wsize = fr_sched.waiting_size();
	} else if (policy == "mlfq") {
		wsize = MLFQ_sched.waiting_size();
	}
	
	return wsize;
}

/* Add a task to the scheduler from a string */
void Scheduler::add_task(string cmd){
	nprocessesadded++;
	process tmp(&cmd[0], timeslice);
	if(policy == "mlfq"){
		MLFQ_sched.add_to_waiting(tmp);
	} else{
		fr_sched.add_to_waiting(tmp);
	}
}

/* deal with messages sent by the client */
string Scheduler::process_command(string client_message) {
	string return_string = "";
	stringstream ss;

	switch (client_message[0]) {
		case 's':
			return_string = print_status();
			break;
		case 'r':
			return_string = print_running();
			break;
		case 'w':
			return_string = print_waiting();
			break;
		case 'f':
			return_string = flush();
			break;
		case 'a':
			/* get whole string after "add " */
			string substr = client_message.substr(4);
			ss << "Added process " << nprocessesadded << ": " << substr << endl;
			return_string = ss.str();
			add_task(substr);
			break;
	}

	return return_string;
}

/* iterate through running processes to preempt and start new process */
void Scheduler::iterate_running_processes() {
	time_t current;
	int status;
	process p;
	for (int i = 0; i< ncpus; i++) {

		// check if there isn't a process, then get it from waiting queue 
		if (running[i].PID == -1) {
			if (policy == "fifo" || policy == "rdrn") {
				running[i] = fr_sched.next_in_waiting();
				// start process if it exists 
				if (running[i].PID != -1) {
					running[i].start();
				} 
			} else if (policy == "mlfq") {
				running[i] = MLFQ_sched.next_in_waiting();
				if (running[i].PID != -1) {
					running[i].start();
				}
			}
		} else {
			status = running[i].update_stat(); // update process info
			if(status == -1){
				process p;
				running[i] = p;
				continue;
			}
			time(&current);

			// If the process is finished running
			if(strcmp(running[i].state_str,"Zombie") == 0){
				nprocessesrun++;

				//Recalculate Avg. Turnaround Time
				time_t turnaround = current - running[i].arrival_t;
				totalTurnaround = totalTurnaround + turnaround;
				avgturn = totalTurnaround/nprocessesrun;

				//Recalculate Avg. Response Time
				time_t response = running[i].start_t - running[i].arrival_t;			
				totalResponse = totalResponse + response;
				avgresp = totalResponse/nprocessesrun;

				//Reap the function
				waitpid(running[i].PID, &status, 0);

				if (policy == "fifo" || policy == "rdrn") {
					running[i] = fr_sched.next_in_waiting();
				} else {
					running[i] = MLFQ_sched.next_in_waiting();
				}	

				if (running[i].PID != -1) {
					running[i].start();
				}

			} else if ((current > running[i].threshold) && (policy != "fifo")) { 
				running[i].stop(); // stop process 
				if (policy == "rdrn") {
					fr_sched.add_to_waiting(running[i]); // add process to waiting queue 
					running[i] = fr_sched.next_in_waiting(); // take the next process from waiting queue 
				} else {
					MLFQ_sched.add_to_waiting(running[i]); 
					running[i] = MLFQ_sched.next_in_waiting();
				}

				if (running[i].PID != -1) {
					running[i].start(); // start process 
				}
			}
		}	
	}

	if (policy == "mlfq" && (current%boost_time == 0) ) {
		MLFQ_sched.boost();
	}
}


/* print running processes */
string Scheduler::print_running() {
	string tmp;
	tmp = print_header();
	for (int i = 0; i< ncpus; i++) {
		if (running[i].PID != -1) {
			tmp = tmp + running[i].format_print();
		}
	}
	return tmp;
}

/* print waiting queue */ 
string Scheduler::print_waiting() {
	string tmp_str;

	if (policy == "fifo" || policy == "rdrn") {
		tmp_str = fr_sched.print_waiting();
	} else if (policy == "mlfq") {
		tmp_str = MLFQ_sched.print_waiting();
	}

	return tmp_str+"\n";
}

/* if client sends "status", print status */
string Scheduler::print_status() {
	int rsize = runningsize();
	int wsize = waitingsize();
	int levels = 0;
	string tmp_str;
	stringstream ss;
	
	if (policy == "mlfq") {
		levels = 5;
	}
		
	ss << "Running =\t" << rsize << ", " << "Waiting =\t" << wsize << ", " << "Levels =\t" << 
	levels << ", " << "Turnaround =\t" << avgturn << ", " << "Response = \t" << avgresp << endl;
	tmp_str = ss.str();

	// print running processes 
	tmp_str = tmp_str + "\nRunning Queue:\n";
	tmp_str = tmp_str + print_running();

	// print waiting processes 
	tmp_str = tmp_str + "\nWaiting Queue:\n";
	tmp_str = tmp_str + print_waiting() + "\n";
	return tmp_str;
}	

/* flush waiting and running processes */
string Scheduler::flush() {
	stringstream ss;
	
	int rsize = runningsize();
	int wsize = waitingsize();
	for (int i = 0; i< ncpus; i++) {
		if (running[i].PID != -1) {
			running[i].end();
			running[i] = process();
		}
	}
	
	if (policy == "fifo" || policy == "rdrn") {
		fr_sched.flush();
	} else if (policy == "mlfq") {
		MLFQ_sched.flush();
	}

	nprocessesrun = 0;
	nprocessesadded = 0;
	totalTurnaround = 0;
	totalResponse = 0;
	avgturn = 0;
	avgresp = 0;
	
	ss << "Flushed " << rsize << " running and " << wsize << " waiting processes" << endl;
	return ss.str();
}
