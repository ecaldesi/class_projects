#include "scheduler.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector> 
#include <fstream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <fnmatch.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_QUEUE 4
#define BUFFSIZE 100
#define PATH "/tmp/pq.socket"

using namespace std;
/* Usage function */

void do_server(int,string,int);
void do_client(string);
vector <string> parse_request(char *);
void int_handler(int signum);

void usage(const char *program_name, int status) {
	fprintf(stderr, "Usage: %s [options]\n", program_name);
	fprintf(stderr, "\nGeneral Options:\n");
	fprintf(stderr, "    -h            	Print this help message\n");
    	fprintf(stderr, "    -f PATH		Path to IPC channel\n");
 
   	fprintf(stderr, "\nClient Options:\n");
    	fprintf(stderr, "    add COMMAND     	Add command to queue\n");
    	fprintf(stderr, "    status		Query status of queue\n");
    	fprintf(stderr, "    running	 	Query running jobs\n");
    	fprintf(stderr, "    waiting		Query waiting jobs\n");
    	fprintf(stderr, "    flush 		Remove all jobs from queue\n");
	
   	fprintf(stderr, "\nServer Options:\n");
    	fprintf(stderr, "    -n CPUS   		Number of CPUs\n");
    	fprintf(stderr, "    -p POLICY   	Scheduling policy (fifo, rdrn, mlfq)\n");
    	fprintf(stderr, "    -t MICROSECONDS	Time between scheduling\n");
    	exit(status);
}


/* Main function */
int main(int argc, char *argv[]) {

	int argind = 1;
    	char *program_name = argv[0];
	
	/* check for arguments*/
	if (argc == 1) {
        	perror("no arguments passed");
        	return 1;
	}

	/* set variables */
	string add = "add ";
	string path, command;
	string policy = "FIFO"; 
	int ncpus = 1;
	int microseconds = 2000;
	bool flag = false;

	/* parse in command line arguments*/
	while (argind < argc && strlen(argv[argind]) > 1) {
		char *arg = argv[argind++];
		switch(arg[0]) {
		    case '-':
			if (arg[1] == 'h') {
				usage(program_name, 0);
			} else if (arg[1] == 'f') {
				if (argind >= argc) {
					cout << "Error! You didn't specify a path" << endl;
					return -1;
				} else {
					path = argv[argind++];
				}
			} else if(arg[1] == 'n') {
				ncpus = atoi(argv[argind++]);
			} else if (arg[1] == 'p') {
				policy = argv[argind++];
			} else if (arg[1] == 't') {
				microseconds = atoi(argv[argind++]);
			}
			break;

		    default:
			flag = true;
			if (strcmp(arg, "add") == 0) {
				if (argind >= argc) {
					cout << "Error! You didn't specify a command" << endl;
					return -1;
				} else {
					command = argv[argind++];
					command = add.append(command);
					while(argind < argc && strlen(argv[argind]) > 0){
						command = command.append(" ");
						arg = argv[argind++];
						command = command.append(arg);
					}
				}
			} else if (strcmp(arg, "status") == 0) {
				command = "status";
			} else if (strcmp(arg, "running")  == 0) {
				command = "running";
			} else if (strcmp(arg, "waiting") == 0) {
				command = "waiting";
			} else if (strcmp(arg, "flush") == 0) {
				command = "flush";
			}else{
				cout << "Invalid Command \n";
				exit(1);
			}
			break;   
		}
	}

	if (!flag){
		do_server(ncpus,(string)policy,microseconds);
	}else{
		do_client(command);
	}
	return 0;
}

void do_server(int cpu, string policy, int slice){
	
	/* Creates Scheduler Object */
		Scheduler main_sched(cpu,policy,slice);

		cpu, slice = 1;
		policy = "fifo";
		/* Initializations */
		struct sockaddr_un server, client;
		char buffer[BUFFSIZE];
		int length,sock,cli,cnt,rv;
		string request;
		string probe = "probe";

		/* Creates Socket */
		if ((sock = socket(AF_UNIX,SOCK_STREAM,0)) == -1){
			fprintf(stderr,"[Socket] : %s \n",strerror(errno));
			exit(1);
		}
	
		/* Builds Server */
		server.sun_family = AF_UNIX;
		strcpy(server.sun_path,PATH);
		unlink(server.sun_path);
		length = strlen(server.sun_path) + sizeof(server.sun_family);

		/* Binds to Socket */
		int n = bind(sock,(struct sockaddr *)&server,length);
		if (n==-1){
			fprintf(stderr,"[Bind] : %s \n",strerror(errno));
			close(sock);
			exit(1);
		}

		/* Designates Socket as Passive */
		if (listen(sock,MAX_QUEUE) == -1){
			fprintf(stderr,"[Listen] : %s \n",strerror(errno));
			exit(1);
		}


		/* Signal Handler */
		signal(SIGINT, int_handler);
	
		/* Accepts */
		socklen_t size = sizeof(client);
		do_client(probe);
		if ((cli = accept(sock,(struct sockaddr *)&client,&size))==-1){
			fprintf(stderr,"[Accept] : %s \n", strerror(errno));
			exit(1);
		}
		shutdown(cli,SHUT_WR);
		struct pollfd fds[1];
		fds[0].fd = sock;
		fds[0].events = POLLIN;
		vector <string> req_type;
		/* Continuously Reads Requests */
		while(true){

			/* Accesses Socket Only When Full */
			rv = poll(fds,1,1000);
			if (rv == -1){
				fprintf(stderr,"[Poll] : %s \n", strerror(errno));
			}else if (rv == 0){
				main_sched.iterate_running_processes();
			}else{
				if (fds[0].revents & POLLIN){

					/* Accepts */
					socklen_t size = sizeof(client);
					if ((cli = accept(sock,(struct sockaddr *)&client,&size))==-1){
						fprintf(stderr,"[Accept] : %s \n", strerror(errno));	
						exit(1);
					}
					/* Reads Bytes */
					bzero(buffer,sizeof(buffer));
					FILE *client_stream = fdopen(cli,"r+");
					if (client_stream == NULL){
						fprintf(stderr,"[fdopen] : %s",strerror(errno));
					}


					request = "";
					req_type.clear();
					if (fgets(buffer,BUFFSIZE,client_stream)){

						/* Parses Request */
						req_type = parse_request(buffer);
						for (int j = 0; j < req_type.size(); j++){
							request.append(req_type[j]);
							request.append(" ");
						}
					}
					fclose(client_stream);
					shutdown(cli,SHUT_WR);
					if ((cli = accept(sock,(struct sockaddr *)&client,&size))==-1){
						fprintf(stderr,"[Accept] : %s \n", strerror(errno));	
						exit(1);
					}
					/* Reads Bytes */
					bzero(buffer,sizeof(buffer));
					client_stream = fdopen(cli,"r+");
					if (client_stream == NULL){
						fprintf(stderr,"[fdopen] : %s",strerror(errno));
					}
						
					if (request.size() > 0){
						cout << "[Request] : " << request << endl; // FOR TESTING
						string tmp_str = main_sched.process_command(request);
						fputs(tmp_str.c_str(),client_stream);
					}else{
						exit(1);	
					}

					fclose(client_stream);
					close(cli);
				}
			}
		}

		/* Closes Sockets*/
		close(sock);
}

void do_client(string cmd){
		/* Initializations */
		struct sockaddr_un server;
		char buffer[BUFFSIZE];
		strcpy(buffer,cmd.c_str());
		int sock, count, length;

		/* Creates Socket Descriptor */
		if ((sock = socket(AF_UNIX,SOCK_STREAM,0))==-1){
			fprintf(stderr,"[Socket] - %s \n", strerror(errno));
			exit(1);
		}
	
		/* Builds Adress */
		server.sun_family=AF_UNIX;	
		strcpy(server.sun_path,PATH);
		length = strlen(server.sun_path) + sizeof(server.sun_family);
		/* Builds Connection */
		if (connect(sock, (struct sockaddr*)&server,length)==-1){
			fprintf(stderr,"[Connect] : %s \n", strerror(errno));
			exit(1);
		}	

		FILE *server_stream = fdopen(sock,"r+");
		if (server_stream == NULL){
			fprintf(stderr,"[fdopen] : %s\n",strerror(errno));
			exit(1);
		}

		fputs(buffer,server_stream);
		fclose(server_stream);
		if (cmd != "probe"){
			close(sock);
		
			/* Creates Socket Descriptor */
			if ((sock = socket(AF_UNIX,SOCK_STREAM,0))==-1){
				fprintf(stderr,"[Socket] - %s \n", strerror(errno));
				exit(1);
			}
			/* Builds Connection */
			if (connect(sock, (struct sockaddr*)&server,length)==-1){
				fprintf(stderr,"[Connect] : %s \n", strerror(errno));
				exit(1);
			}	

			server_stream = fdopen(sock,"r+");
			if (server_stream == NULL){
				fprintf(stderr,"[fdopen] : %s\n",strerror(errno));
				exit(1);
			}


			while (fgets(buffer,BUFFSIZE,server_stream)){
				cout << buffer;	
			}
			fclose(server_stream);
		}
		/* Close Socket Before Exiting */
		close(sock);
}



vector <string> parse_request(char * buffer){
	vector <string> tmp;
	char * token;
	token = strtok(buffer," ");
	while (token != NULL){
		tmp.push_back((string)token);
		token = strtok(NULL," ");
	}
	return tmp;
}

void int_handler(int signum) {
	cout << endl << "Bye " << endl;
	exit(EXIT_FAILURE);
}
