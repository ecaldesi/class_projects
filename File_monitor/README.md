# Rorschach, File Monitor
By Elisabetta Caldesi, Anthony DiFalco, Owen Phelan <br />

This program scans a root directory for any changes to files underneath this folder and executes user-specified actions on the files based on pattern matching rules. <br />

EVENTS
------- 
Rorschach will examine all the files under the root directory and any sub-directories (ie. nested directories), to determine if any of the following file system events occurred:
- CREATE: A file has been created under the monitored directory.
- MODIFY: A file has been modified under the monitored directory.
- DELETE: A file has been deleted under the monitored directory.

RULES
--------
Rorschach will load in pattern rules from the file rules unless otherwise specified by the -f flag as shown below: <br />
EVENT PATTERN ACTION

ENVIRONMENT VARIABLES
----------------------
Rorschach must pass the following environmental variables to specified command:
- BASEPATH: This is the base path of the file (i.e. without any proceeding extensions).
- FULLPATH: This is the full path of the file.
- EVENT: This is the type of event detected.
- TIMESTAMP: This is the current timestamp.

USAGE 
---------
$ ./rorschach -h <br />
Usage: rorschach options ROOT <br />

Options:

    -h          Print this help message
    -f RULES    Load rules from this file (default is rules)
    -t SECONDS  Time between scans (default is 5 seconds)
