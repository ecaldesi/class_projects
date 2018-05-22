# Process Queue
By Elisabetta Caldesi, Anthony DiFalco, Owen Phelan

Project whose job is to receive a series of jobs and then schedule them to run on multiple CPU cores as separate processes. <br />

This process queue has two components:
- Client: This component allows the user to send the following commands to the server:
  - Add: This adds a command to the waiting queue on the server.
  - Status: This retrieves and displays the status of the queue.
  - Running: This retrieves and displays just the running jobs in the queue.
  - Waiting: This retrieves and displays just the waiting jobs in the queue.
  - Flush: This removes all the jobs from the queue (and terminates any active processes).
- Server: This component maintains of a queue of running jobs (ie. tasks that actively assigned CPU time), and waiting jobs (ie. tasks that haven't started yet or have been preempted). Its purpose is to schedule as many running jobs as there are CPU cores using one of the three scheduling policies mentioned below.

Scheduling 
------------
- FIFO: Execute jobs in the order in each they arrival in the process queue.
- Round Robin: Execute jobs in a round robin way.
- Multi-Level Feedback Queue: Execute jobs based on a dynamic priority structure.

Usage
-----
$ ./pq -h
Usage: ./pq [options]

General Options:

    -h                 Print this help message
    -f PATH            Path to IPC channel

Client Options:

    add COMMAND        Add COMMAND to queue
    status             Query status of queue
    running            Query running jobs
    waiting            Query waiting jobs
    flush              Remove all jobs from queue

Server Options:

    -n NCPUS           Number of CPUs
    -p POLICY          Scheduling policy (fifo, rdrn, mlfq)
    -t MICROSECONDS    Time between scheduling
