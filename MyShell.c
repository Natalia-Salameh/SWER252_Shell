#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#define clear() printf("\033[H\033[J")

void Intro(){
  puts("\n ~Welcome to Natalia's Shell~\n");
  sleep(1);
  clear();
}

void help() {
  puts("\nTry these Available Commands: :)"
 "\n a) pwd command: gets the current working directory."
 "\n b) cd command: changes the directory depending on users input." 
 "\n c) ps command: lists small subsets of the current running processes on the system."
 "\n ps -A: Lists all the running processes on the system."
 "\n d)kill command: kills the signal by kill PID or kill name."
 "\n e)history: gets the recent entered shell commands."
 "\n f)exit: exit the shell."
 "\n g)clear: clears the shell."
 "\n h)You can view your shell command history by pressing on the up/down key\n");
}

void pwd() {
  char cwd[700]; /*Size of the current working directory is 700*/
  printf("%s\n", getcwd(cwd, 700));
}

void cd(char ** args) {

  /*Check if the path is null*/
  if (args[1] == NULL) {
    /*Go back to home directory if null*/
    chdir(getenv("HOME"));

  } else if (chdir(args[1]) == -1) {
    printf("%s: No such file or directory\n", args[1]);
  } else chdir(args[1]);
}

void ps() {
  char flag, * t; /*T,F*/
  char cmd[700], tty_self[700], path[700], time_s[700];
  struct dirent * dir; /*returns dir entries*/
  int i, fd;
  unsigned long time, stime;

  printf("%6s %s\t%8s %s\n", "PID", "TTY", "TIME", "CMD");

  /*Open Directory*/
  DIR * d = opendir("/proc");
  if (d == NULL) {
    perror("DIR: Unable to open directory");
    exit(1);
  }

  /*Read file*/
  int fd_self = open("/proc/self/fd/0", O_RDONLY);

  sprintf(tty_self, "%s", ttyname(fd_self));

  /*Read directory*/
  while ((dir = readdir(d)) != NULL) {
    flag = 1;
    for (i = 0; dir -> d_name[i]; i++) {
      /*null terminated filen*/
      if (!isdigit(dir -> d_name[i])) {
        flag = 0;
        break;
      }
    }

    if (flag) {
      /*PID*/
      sprintf(path, "/proc/%s/fd/0", dir -> d_name);

      fd = open(path, O_RDONLY);
      /*TTY*/
      if (ttyname(fd) && strcmp(ttyname(fd), tty_self) == 0) {

        sprintf(path, "/proc/%s/stat", dir -> d_name);

          FILE *f = fopen(path, "r"); /*Opens a file for reading*/

        /*CMD*/
        fscanf(f, "%d%s%c%c%c", & i, cmd, & flag, & flag, & flag);
        /*Remove Right brucket*/
        cmd[(strlen(cmd) - 1)] = '\0';

        for (i = 0; i < 11; i++) {
          fscanf(f, "%lu", & time);
        }
        fscanf(f, "%lu", & stime);

        time = (int)((time + stime) / sysconf(_SC_CLK_TCK));

        sprintf(time_s, "%02lu:%02lu:%02lu", (time / 3600) % 3600, (time / 60) % 60, time % 60);

        /*Print output*/
        printf("%5s %s\t%8s %s\n", dir -> d_name, ttyname(fd) + 5, time_s, cmd + 1);

      }
    }
  }
}


void killc(char ** args) {
    pid_t pid;
    int p,n;
    int i = 1;
    char *name;
    
    while(args[i] != NULL){
    if(isdigit(*args[i])){
    pid = atoi(args[i]); /*Convert entered pid from string to int*/
    p = kill(pid, SIGTERM); /*kill the signal*/
      if (p == 0) {
      printf("(pid: %d) -killed\n", pid);
    } else {
    //perror("kill");
    printf("%s: (%d) -No such process\n", args[0], pid);
    } 
    }else{
    	name = args[i]; /*kill the signal by name*/
    n = kill(*name,SIGTERM);
    if (n == 0) {
      printf("(pid: %s) -killed\n", name);
    } else {
    //perror("kill");
    printf("%s: (%s) -No such process\n", args[0], name);
    } 
   }
    i++;
  } 
}

void exec(char ** args) {
  if (strcmp(args[0], "exit") == 0) {
    clear();
    exit(0);
  } /*Check if the command is pwd*/
  else if (strcmp(args[0], "pwd") == 0) {
    pwd();
    /*Check if the command is cd*/
  } else if (strcmp(args[0], "cd") == 0) {
    cd(args);
  } else if (strcmp(args[0], "ps") == 0) {
    if (args[1] == NULL) {
      /*To list a brief of the processes running on the system*/
      ps();
    } else if (strcmp(args[1], "-A") == 0) {
      /*To list every process running on the system*/
      system("ps -A");
    }
  } else if (strcmp(args[0], "help") == 0) {
    help();
  } else if (strcmp(args[0], "clear") == 0) {
    clear();
  } else if (strcmp(args[0], "kill") == 0) {
    killc(args);
  } else {
    printf("%s: command not found\n", args[0]);
  }

}

char ** split_line(char * line) {
  int length = 0;
  int capacity = 16;

  char ** tokens = malloc(capacity * sizeof(char * ));
  if (!tokens) {
    perror("Myshell: Malloc");
    exit(1);
  }

  char * delimiters = " \t\r\n";
  char * token = strtok(line, delimiters);

  while (token != NULL) {
    tokens[length] = token;
    length++;

    if (length >= capacity) {
      capacity = (int)(capacity * 1.5);
      tokens = realloc(tokens, capacity * sizeof(char * ));
      if (!tokens) {
        perror("Myshell");
        exit(1);
      }
    }

    token = strtok(NULL, delimiters);
  }

  tokens[length] = NULL;
  return tokens;
}

int main() {
  clear();
  Intro();
  while (1) {
    char cmd[700];
    printf("%s@%s", getenv("LOGNAME"), getcwd(cmd, 700));

    char * line = readline("$ ");
        if(line != NULL)
    	add_history(line);
    char ** tokens = split_line(line);
    
    add_history(line);

    if (tokens[0] != NULL) {
      exec(tokens);
    }

    free(tokens);
    free(line);
  }
}
