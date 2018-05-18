pq: 			./process/main.cpp scheduler.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o process.o utils.o
			g++ -std=c++0x -Wno-write-strings pq.cpp scheduler.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o process.o utils.o -o pq

processTest: 		./process/main.cpp process.o utils.o
			g++ -std=c++0x -Wno-write-strings ./process/main.cpp utils.o process.o -o processTest	

schedulerTest: 		scheduler_test.cpp scheduler.o process.o utils.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o
			g++ -std=c++0x -Wno-write-strings scheduler_test.cpp scheduler.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o process.o utils.o -o schedulerTest

schedulerTest2:         scheduler_test2.cpp scheduler.o process.o utils.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o
			g++ -std=c++0x -Wno-write-strings scheduler_test2.cpp scheduler.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o process.o utils.o -o schedulerTest2

scheduler.o: 		scheduler.cpp process.o utils.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o
			g++ -std=c++0x -Wno-write-strings -c scheduler.cpp

FIFORDRN_Scheduler.o:	FIFORDRN_Scheduler.cpp process.o utils.o
			g++ -std=c++0x -Wno-write-strings -c FIFORDRN_Scheduler.cpp

MLFQ_Scheduler.o: 	MLFQ_Scheduler.cpp process.o utils.o
			g++ -std=c++0x -Wno-write-strings -c MLFQ_Scheduler.cpp

process.o: 		./process/process.cpp 
			g++ -std=c++0x -Wno-write-strings -c ./process/process.cpp 

utils.o: 		./process/process.cpp ./process/utils.cpp
			g++ -std=c++0x -Wno-write-strings -c ./process/utils.cpp 

clean:
			rm process.o utils.o scheduler.o FIFORDRN_Scheduler.o MLFQ_Scheduler.o
			rm processTest schedulerTest pq schedulerTest2

