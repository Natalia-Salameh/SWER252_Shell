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
#include<readline/readline.h>
#include<readline/history.h>
#define clear() printf("\033[H\033[J")

void help() {
  puts("\nTry these are the Available Commands: :)"
    "\n-(pwd) (cd) (ps) (kill) (exit)-"
  );

}

void pwd() {
  char cwd[700]; /*Size of the current working directory is 700*/
  printf("%s\n", getcwd(cwd, 700));
}

void cd(char ** args) {

  if (args[1] == NULL) {
    /*Check if the path is null*/
    chdir(getenv("HOME")); /*Go back to home directory if null*/

  } else if (chdir(args[1]) == -1) {
    /*Take the path from the user*/
    printf("%s: No such file or directory\n", args[1]);
  }
}

void ps() {
  char flag, * t;
  char cmd[700], tty_self[700], path[700], time_s[700];
  FILE * f;
  DIR * d;
  struct dirent * dir;
  int i, fd_self, fd;
  unsigned long time, stime;

  /*Open Directory*/
  d = opendir("/proc");
  /*Open file directory*/
  fd_self = open("/proc/self/fd/0", O_RDONLY);

  sprintf(tty_self, "%s", ttyname(fd_self));

  printf("%5s %s\t%8s %s\n", "PID", "TTY", "TIME", "CMD");

  /*Read file*/
  while ((dir = readdir(d)) != NULL) {

    flag = 1;
    for (i = 0; dir -> d_name[i]; i++) {
      if (!isdigit(dir -> d_name[i])) {
        flag = 0;
        break;
      }
    }

    if (flag) {

      sprintf(path, "/proc/%s/fd/0", dir -> d_name);

      fd = open(path, O_RDONLY);

      if (ttyname(fd) && strcmp(ttyname(fd), tty_self) == 0) {

        sprintf(path, "/proc/%s/stat", dir -> d_name);

        f = fopen(path, "r");

        fscanf(f, "%d%s%c%c%c", & i, cmd, & flag, & flag, & flag);

        cmd[strlen(cmd) - 1] = '\0';

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


void exec(char ** args) {
  if (strcmp(args[0], "exit") == 0){
  clear();
   exit(0);
   }
  else if (strcmp(args[0], "pwd") == 0) {
    /*Check if the command is pwd*/
    pwd();
  } else if (strcmp(args[0], "cd") == 0) {
    /*Check if the command is cd*/
    cd(args);
  } else if (strcmp(args[0], "ps") == 0) {
    if (args[1] == NULL) {
      ps();
    } else if (strcmp(args[1], "-A") == 0) {
      system("ps -A");
    }
  } else if (strcmp(args[0], "help") == 0) {
    help();
  } else {
    printf("command '%s' not found\n", args[0]);
    }

  pid_t child_pid = fork();

  if (child_pid == 0) {
    exit(1);
  } else if (child_pid > 0) {
    int status;
    do {
      waitpid(child_pid, & status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
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

char * read_line() {

  char * line;
  size_t buflen = 0;
  errno = 0;
  ssize_t strlen = getline( & line, & buflen, stdin);
  if (strlen < 0) {
    if (errno) {
      perror("Myshell");
    }
    exit(1);
  }

  return line;
}

int main() {
  clear();
  while (1) {

    char cmd[700];
    printf("%s@%s", getenv("LOGNAME"), getcwd(cmd, 700));

    char * line = readline("$ ");
    char ** tokens = split_line(line);

    rl_bind_key('\t', rl_complete);
    add_history(line);

    if (tokens[0] != NULL) {
      exec(tokens);
    }

    free(tokens);
    free(line);
  }
}
