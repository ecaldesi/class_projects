#include "fs_monitor.h"
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream> 
#include <map>
#include <cstring> 
#include <iomanip>
#include <unistd.h>
#include <cstring>

using namespace std;


/* fs_monitor functions -----------------------------------------------*/
string assemble_path(string dir_str, char* file_str){
	stringstream ss;
	ss << dir_str << "/" << file_str;
	return ss.str();
}

//Constructor
fs_monitor::fs_monitor(string path_to_root_dir){
	full_root_path = path_to_root_dir;

	construct_dir(full_root_path);	
};

void fs_monitor::construct_dir(string path_to_dir){
	// struct to store directories to be checked

	// Open up the Directory Contents
   DIR *d = opendir(&path_to_dir[0]);
   if(d){

   	struct dirent *dir;

   	// For each file in the directory we extract info and store it in the map
  		while ((dir = readdir(d)) != NULL) {
  				if( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") ){
	  				// Get the File Path 
	  				string file_path = assemble_path(path_to_dir, dir->d_name);

	  				// Get the modification time
	  				struct stat s;
	  				if(stat(&file_path[0], &s) < 0){
	  					cout << "error, could not stat the file - " << file_path << endl;
	  				}
	  				time_t mod_t = s.st_mtime;

	            // Get the file type. if directory, add to stack
	  				mode_t file_type = s.st_mode;
	            if (S_ISDIR(file_type)){
	            	directories.push_back(file_path);
	            }	

	            // Add file information to tree
	            node temp = { .path=file_path, .inode_num=dir->d_ino, .mod_time=mod_t, .in_fs=false};
	            files.insert(pair<ino_t, node>(temp.inode_num, temp));	

         	}// end if for "."" and ".." check
        } // end while for directory read 	
   } // end if "d" opened the directory

   closedir(d);

   while(!directories.empty()){
   	string dir_str = directories.back();
   	directories.pop_back();
   	construct_dir(dir_str);
   }
}

void fs_monitor::print_files(void){
  // show content:
  	for (file_it=files.begin(); file_it!=files.end(); file_it++)
    	file_it->second.print();
}

void fs_monitor::print_events(void){
  	for (event_it=events.begin(); event_it!=events.end(); event_it++){
    	cout << event_it->first << " :: " << event_it->second << endl;
    }	
}

void fs_monitor::scan(string dirpath = ""){
	vector<pair<string,node> > tmp;
	string path;
	string event;
	if(dirpath == ""){
		path = full_root_path;
	}
	else{
		path = dirpath;
	}

	// Open up the Directory Contents
   DIR *d = opendir(&path[0]);
   if(d){

   	struct dirent *dir;

   	// For each file in the directory we check the info against the map
  		while ((dir = readdir(d)) != NULL) {
  				if( strcmp(dir->d_name, ".") && strcmp(dir->d_name, "..") ){
	  				// Get the File Path 
	  				string file_path = assemble_path(path, dir->d_name);
	  				// Get the modification time
	  				struct stat s;
	  				if(stat(&file_path[0], &s) < 0){
	  					cout << "error, could not stat the file " << file_path << endl;
	  					continue;
	  				}
	  				time_t mod_t = s.st_mtime;

	            // Get the file type. if directory, add to stack
	  				mode_t file_type = s.st_mode;
	            if (S_ISDIR(file_type)){
	            	directories.push_back(file_path);
	            }	

	            // See if the file has changed since last time
	            event = check_change(dir->d_ino, mod_t, file_path);
	            if(event != ""){
	            	events.push_back(pair<string,string>(event,file_path));
	            }

         	}// end if for "."" and ".." check
        } // end while for directory read 	
   } // end if "d" opened the directory

   closedir(d);

   while(!directories.empty()){
   	string dir_str = directories.back();
   	directories.pop_back();
   	scan(dir_str);
   }


   if(path == full_root_path){
	   for(file_it = files.begin(); file_it != files.end(); file_it++){
	   	if(file_it->second.in_fs == false){
	   		cout << "\nDetected \"DELETE\" event on \""<< file_it->second.path << "\"\n";
	   		events.push_back(pair<string,string>("DELETE", file_it->second.path));
	   		files.erase(file_it);
	   	}
	   	else{
	   		file_it->second.in_fs = false;
	   	}
	   }
	}
}

string fs_monitor::check_change(ino_t test_inode, time_t test_mod_time, string test_name){
	// Look for relevant inode
  	file_it = files.find(test_inode);

  	// If it's not in the map -> Create event
  	if (file_it == files.end()){
	   // Add file information to tree
	   node temp = { .path=test_name, .inode_num=test_inode, .mod_time=test_mod_time, .in_fs=true};
	   files.insert(pair<ino_t, node>(temp.inode_num, temp));
      cout << "\nDetected \"CREATE\" event on \"" << test_name << "\"\n";
	   return "CREATE";

  	}
  	// If it is in the map...
	else{

		// add record of file being in system
		file_it->second.in_fs=true;
		// check if mod time has changed -> Modification Event
		if (file_it->second.mod_time != test_mod_time){
			file_it->second.mod_time=test_mod_time;
      	cout << "\nDetected \"MODIFY\" event on \"" << test_name << "\"\n";
			return "MODIFY";
		}
	}

	return "";
}
