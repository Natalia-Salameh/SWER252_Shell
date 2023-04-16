# SWER252_Shell

This is a simple Unix shell written in C. 

It includes the following required commands: 
a) pwd command: gets the current working directory.\n
b) cd command: changes the directory depending on users input.
c) ps command: lists small subsets of the current running processes on the system.
   ps -A: Lists all the running processes on the system.
d)kill command: kills the signal by kill PID or kill name.

In addition to the required commands, the shell also includes some extra features:
a)history: gets the recent entered shell commands. 
b)help: menu of available commands to use. 
c)intro: welcoming intro to the shell.
d)exit: exit the shell.
e)clear: clears the shell.

# How to use
You can launch the shell by executing the following command:
gcc MyShell.c -o MyShell -lreadline

# Credits
This shell was developed by Natalia Salameh (202006019). 

Used the following resources as references:
resources: 1)https://github.com/patoliyam/Basic-Shell-Implementation-in-C/blob/master/group-29-c-shell.c
           2)https://brennan.io/2015/01/16/write-a-shell-in-c/
           3)https://github.com/dmulholl/kash/blob/master/src/kash.c
           4)https://github.com/deepakrenni/PS-/blob/master/ps%20Implementation%20using%20C
           5)https://gist.github.com/DendiBot/ed66adfb03c1d000c4a746756900ba1a
           

