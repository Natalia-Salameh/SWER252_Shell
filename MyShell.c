#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

void pwd() {
  char cwd[700]; /*Size of the current working directory is 700*/
  printf("Your current working directory is: %s\n", getcwd(cwd, 700));
}

void cd(char ** args) {
  if (args[1] == NULL) {
    /*Check if the path is null*/
    chdir(getenv("HOME")); /*Go back to home directory if null*/

  } else {
    if (chdir(args[1]) == -1) {
      /*Take the path from the user*/
      printf("%s: No such file or directory\n", args[1]);
    }
  }
}

void exec(char ** args) {

  if (strcmp(args[0], "pwd") == 0) {
    /*Check if the command is pwd*/
    pwd();
  }

  if (strcmp(args[0], "cd") == 0) {
    /*Check if the command is cd*/
    cd(args);
  }

  pid_t child_pid = fork();

  if (child_pid > 0) {
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
  char * line = NULL;
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
  while (1) {

    char cwd[700]; /*size of the current working directory is 700*/
    /*get the current working directory and show it in the prompt*/
    printf("%s@%s$ ", getenv("LOGNAME"), getcwd(cwd, 700));
    char * line = read_line();
    char ** tokens = split_line(line);

    if (tokens[0] != NULL) {
      exec(tokens);
    }

    free(tokens);
    free(line);
  }
}
