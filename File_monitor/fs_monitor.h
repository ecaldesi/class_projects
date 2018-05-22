#ifndef  FS_MONITOR_H
#define	FS_MONITOR_H

#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream> 
#include <map> 
#include <iomanip>
#include <unistd.h>

using namespace std;

struct node{
	string path;
	ino_t inode_num;
	time_t mod_time;
	// When the tree is constructed "in_fs" is set to false. During scan, if a file is still in the filesystem
	// it is set to true. Then, when looking for deletions, set back to false for the next scan. 
	bool in_fs;

	void print(void){
		cout << setw(40) << path << setw(50) << "//inode# - " << inode_num << "//mod_time - " << mod_time << endl;
	}
};

class fs_monitor{

	public:
		fs_monitor(string);
		void construct_dir(string);
		void scan(string);
		void print_files(void);
		void print_events(void);
		string get_root(void){ return full_root_path;}
		string check_change(ino_t, time_t, string);
		
		vector<pair<string,string> > events;
		vector<pair<string,string> >::iterator event_it;

	private:
		string full_root_path;
		vector<string> directories;
		map<ino_t, node> files;
		map<ino_t, node>::iterator file_it;

};


#endif