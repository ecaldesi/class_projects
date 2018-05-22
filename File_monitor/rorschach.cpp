/* rorschach.cpp */
#include "fs_monitor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector> 
#include <fstream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <fnmatch.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Functions */

// Strips URL
string getBasePath(string s){

    char sep = '/';
    size_t found = s.rfind(sep, s.length());

    if (found != string::npos) {
        return(s.substr(found+1, s.length() - found));
    }
  
    return(""); 
}

// Matches Glob
int match (string s, string b){
    string addition = ".";
    addition.append(b);
    regex e(addition);
    if (regex_search(s,e)){
        return 0;
    }else{
        return -1;
    } 
};

struct rules {
      string EVENT;
      string PATTERN;
      string ACTION;
};

// Executes filestem action w/ rules
int execute(string path, string event, vector <rules> &rulez){   

   // Initializations
   auto it = rulez.begin();
   bool flag = false;
   string BASEPATH = getBasePath(path); 
   string FULLPATH = path;
  
   // Tests modified file w/ rules DS
   while (it != rulez.end()) {
      flag = false;
      int i = fnmatch(&it->PATTERN[0], &FULLPATH[0], 0);
      int j = strcmp(&event[0], &it->EVENT[0]);

    // File or Dir Created
     // Matches Glob w/ Fullpath & Basepath
     if ((event== "CREATE") && (i == 0) && (j == 0)){
         cout << "Matched \""<< it->PATTERN<< "\" on \"" << BASEPATH << "\"\n";
         cout << "Executing action \""<< it->ACTION<< "\" on \"" << BASEPATH << "\"\n";  
         flag = true;
      }     

    // File or Dir has been Modified
     // Matches Glob w/ Fullpath & Basepath
     if ((event == "MODIFY") && (i == 0) && (j == 0)){
         cout << "Matched \""<<it->PATTERN<<"\" on \""<<BASEPATH << "\"\n";
         cout << "Executing action \""<<it->ACTION<<"\" on \""<<BASEPATH << "\"\n";
         flag = true;
     }

     // Matches Glob w/ Fullpath & Basepath
     if ((event == "DELETE") && (i == 0) && (j == 0)){
         cout << "Matched \""<<it->PATTERN<<"\" on \""<<BASEPATH << "\"\n";
         cout << "Executing action \""<<it->ACTION<<"\" on \""<<BASEPATH << "\"\n";
         flag = true;
     }           
    
      /* ACTUAL EXECUTION */
    if (flag == true){
        const char* ev = it->EVENT.c_str();
        const char* arg5 = it->ACTION.c_str();
        string arg = "/bin/sh";
        string arg1= "-c";
        const char* arg3 = arg.c_str();
        const char* arg4 = arg1.c_str();
      
        string temp = "time";

        // Sets ENV vars
        setenv("FULLPATH", &FULLPATH[0], 1);
        setenv("BASEPATH", &BASEPATH[0], 1);
        setenv("EVENT", ev, 1);
        setenv("TIMESTAMP", &temp[0], 1);

   
        //Execution Sequence
        pid_t pid = fork();

        // Failed
        if (pid == -1){
            cout << "Failed while forking in execute \n";
            return -1;
  
        // Waiting on Child
        } else if (pid > 0){
          int status;
          waitpid(pid,&status,0);

        // Child Executes
        } else {
          execl(arg3, arg3, arg4, arg5, NULL); 
          return -1;
        }
    }
      it++;
   }  

  return 0;
};


/* MAIN FUNCTIONS ----------------------*/
void usage(const char *program_name, int status) {
    fprintf(stderr, "Usage: %s [option] ROOT\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -h              Print this help message\n");
    fprintf(stderr, "    -f RULES        Load rules from this file (default is rules)\n");
    fprintf(stderr, "    -t SECONDS      Time between scans (default is 5 seconds)\n");
    exit(status);
}

void alarm_signal(int signum) {
	cout << "Cleaning up" << endl;
        cout << "Bye" << endl;;
	exit(EXIT_FAILURE);
}

/* Main Execution */

int main(int argc, char *argv[]) {

    // program name
    int argind = 1;
    char *program_name = argv[0];
    
    // if no argument is passed
    if (argc == 1) {
        perror("no arguments passed");
        return 1;
    }
    
    
    string root = "."; /* set root */
    string rulesfile = "rules.txt"; /* use default file rules.txt*/

    int timeout = 5;
    
    struct stat filestat;
    // read in command line args
    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char *arg = argv[argind++];
        switch(arg[1]) {
            case 'h':
                usage(program_name, 0);
                break;

            case 'f':
                lstat(argv[argind], &filestat);
                if (S_ISREG(filestat.st_mode)) {
                    rulesfile = argv[argind++];
                } else if(S_ISDIR(filestat.st_mode)) {
                    cout << "directorY" << endl;
                } else {
                    perror("the rules aren't valid");
                }
                break;   

            case 't':
                timeout = atoi(argv[argind++]);
                break;                
        }
    }
    if (argc%2 == 0) {
        root = argv[argc-1];
    }
    string temp;
    vector <rules> rulesvector;

    /* open file */
    ifstream file;
    string line;
    file.open(rulesfile);
    int count;

    if (!file){
        cout << "error: could not read rules" << endl;
        return 0;
    }

    while (file){
        string tmp;
        getline(file,line);
        istringstream iss(line);
        rules buffer;
        count = 1;
        while (iss >> tmp){
            if(count == 1){
		buffer.EVENT=tmp;
            } else if(count == 2){
	        buffer.PATTERN=tmp;
            } else{
                temp.append(tmp);
		temp.append(" ");
            }
            count++;
        }	     
        
        
        buffer.ACTION = temp;
        rulesvector.push_back(buffer);
        temp = "";
        iss.str("");
    }
    rulesvector.pop_back();

    // Set up the filesystem monitor 
    cout << "Monitoring " << root << endl;
    fs_monitor monitor = fs_monitor(root);
    int timer = timeout;
   
    while(true){
        sleep(1);
        if(timer <=0){
            monitor.scan("");
            while(!monitor.events.empty()){
                string tmp_event = monitor.events.back().first;
                string tmp_path = monitor.events.back().second;
                monitor.events.pop_back();
                execute(tmp_path,tmp_event,rulesvector);
                signal(SIGINT, alarm_signal);
            }
            timer = timeout;
        }
        timer--;
    }
   
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
