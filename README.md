# SWER252_Shell

This is a simple Unix shell written in C. 

It includes the following required commands: 
- pwd command: gets the current working directory.\n
- cd command: changes the directory depending on users input.
- ps command: lists small subsets of the current running processes on the system.
   ps -A: Lists all the running processes on the system.
-kill command: kills the signal by kill PID or kill name.

In addition to the required commands, the shell also includes some extra features:
- history: gets the recent entered shell commands. 
- help: menu of available commands to use. 
- intro: welcoming intro to the shell.
- exit: exit the shell.
- clear: clears the shell.

## How to use
You can launch the shell by executing the following command:
gcc MyShell.c -o MyShell -lreadline

## Credits
This shell was developed by Natalia Salameh (202006019). 

Used the following resources as references:
resources: - https://github.com/patoliyam/Basic-Shell-Implementation-in-C/blob/master/group-29-c-shell.c
           - https://brennan.io/2015/01/16/write-a-shell-in-c/
           - https://github.com/dmulholl/kash/blob/master/src/kash.c
           - https://github.com/deepakrenni/PS-/blob/master/ps%20Implementation%20using%20C
           - https://gist.github.com/DendiBot/ed66adfb03c1d000c4a746756900ba1a
           

