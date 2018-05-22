CSE.30341.FA17: Project 01
==========================

This is the documentation for [Project 01] of [CSE.30341.FA17].

[Project 01]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project01.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/

Members
------------
1. Elisabetta Caldesi (ecaldesi@nd.edu) 
2. Anthony DiFalco (adifalco@nd.edu)
3. Owen Phelan (ophelan1@nd.edu)

Design
---------
Explain how you would periodically scan the `root` the 
directory and detect if a file has been created, modified, or removed.
What system calls would you need to use?

	- Opendir
	- Readdir
	- Closedir
	- lstat

What information would you need to store and what data structures would you need?
	Info

		Name and Location of each File 
		Used to determine if a file has been created or deleted

	Last Modify Time

		Used to check if the file has been changed 

	Data Structure 

		It makes sense to store the above data as a tree structure, 
because a tree naturally mirrors the organization of a file system. 


Explain how you would check if a rule matched an event and execute the corresponding action.

	Detect Events 

	Create/Delete - If a file is found during a traversal that 
is not yet in the data structure , this means that file has been created 
since the last scan. If a file that existed is missing, it has been deleted

	Modify - If a file’s last modified time has changed (is 
now more recent), then is has been modified

	Check Against Rules

	After getting the rules text file, we will parse it and store 
the rules in a data structure. Later on the program will match the regex 
contained in the data structure with the events that occurred. 

	Execute Action

	The necessary actions should be executed using a fork and exec 
command. This allows for any action to be specified in the list of rules 
and also protects the parent program from crashing due to errors caused 
by these actions. 

What system calls would you use? 
	- Fork
	- Exec
	- Wait

How would you pass the environment variables to the command?
	- Basepath & Fullpath - Basepath will be constructed while 
traversing the tree data structure that will hold the file system 
info. Fullpath can be found using a specific file or directory’s information. 

	- Type of event will be found using the procedure described above. 
If a new file exists, it’s a CREATE event. If one is missing, it’s a 
DELETE. If the modify time has changed, it’s a MODIFY. 

	- The current time can be found using a system call 
		- Basepath and fullpath: Manually set the variables 
		  with functions that can be used to combine string
		- Event: read in from the rules file 
		- Timestamp: read in from the command line. 

Explain how you would handle a [SIGINT] signal and what you would need to cleanup?
What system calls would you need to use?
	signal()

How would you know what resources to cleanup?
	There are two data structures that will contain manually allocated 
data. The first is the tree that will hold the information about the filesystem.  
We will know what resources to clean up by recursively traversing the tree, and 
freeing allocated data from the bottom up. Each parent node will delete each of 
its children by calling the cleanup function recursively, until it has none left, 
and then will free itself and return. The second data structures needs to hold 
the rules information. Because the program will need to traverse every rule regardless, 
this can be stored in a linked list. Linked lists can also be cleaned up in a 
similar recursive fashion as outlined above. 

We also attempted to test it with demo.py
.

Testing
-------

Describe how you tested and verified that `rorschach` met 
the project requirements.

In order to test each function, we created little tests for 
each function and tested it individually so we wouldn't have 
to test every function all together because it's hard to find the error when there's so many functions that might not work. We also used demo.py 


Analysis
--------
 
How is what `rorschach` does similar to 
how a [system call] operates?

	It is similar to how a system call operates 
because it does file manipulation as it opens and 
reads the rules file and it performs process control since 
it forks, executes/waits and waits for a signal.


As described in the project requirements, `rorschach` 
periodically scans the `root` directory to detect changes 
in files.  This process can be described as a form of [busy waiting]

1. In this specific context, why could this be considered a possible 
negative design flaw?

	Because the program keeps running recursively through 
directories to detect events, which is time consuming. 


2. Consider [inotify(7)] provided by [Linux].  How would these 
system calls help address the problem of [busy waiting]?

	Because the inotify system calls are used to to keep a 
watch list of the events going on in the directory, and once something 
happens we are immediately notified

3. Why might we still consider using [busy waiting] as specified in 
the original design rather than use something like [inotify(7)]?

	The cache sometimes might be inconsistent with the filesystem, 
so a program that does consistent checking and updates the cache is 
more reliable. 

[Linux]:        https://kernel.org
[busy waiting]: https://en.wikipedia.org/wiki/Busy_waiting
[system call]:  https://en.wikipedia.org/wiki/System_call
[inotify(7)]:   http://man7.org/linux/man-pages/man7/inotify.7.html


Errata
------

Describe any known errors, bugs, or deviations from the requirements.

We pass some tests but when it comes to the exec test for example, we see that our program is doing the right things by what it is displaying, so we believe we're not testing for some edge cases that we should have accounted.

Grader
------


- Luke Duane (lduane@nd.edu)
