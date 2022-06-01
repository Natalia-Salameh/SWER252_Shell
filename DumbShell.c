#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
#define      DEBUG        1
#define      MAXLINELEN   4096
#define      MAXARGS             128
#define      END_OF_LINE  0
#define      SEQ_OP       ';'
#define      SEQUENCE     1

struct cmd {
struct cmd   *next;
int          terminator;
char         *exe_path;
int          nargs;
char         *arg[MAXARGS];
};

void   *ck_malloc(size_t   size)
{
void   *ret = malloc(size);
if (!ret) {
perror("dumbshell:ck_malloc");
exit(1);
}
return ret;
}

char   *skip_to_non_ws(char       *p)
{
int    ch;
while (ch = *p) {
if (ch != ' ' && ch != '\t' && ch != '\n') return p;
++p;
}
return 0;
}

char   *skip_to_ws_or_sep(char    *p)
{
int    ch;
while (ch = *p) {
if (ch == ' ' || ch == '\t' || ch == '\n') return p;
if (ch == SEQ_OP) return p;
++p;
}
return 0;
}

struct cmd *parse_commands(char  *line)
{
char         *ptr; int          ix; struct cmd   *cur;

ptr = skip_to_non_ws(line);
if (!ptr) return 0;
cur = ck_malloc(sizeof *cur);
cur->next = 0;
cur->exe_path = ptr;
cur->arg[0] = ptr;
cur->terminator = END_OF_LINE;
ix = 1;
for (;;) {
ptr = skip_to_ws_or_sep(ptr);
if (!ptr) {
break;
}
if (*ptr == SEQ_OP) {
*ptr = 0;
cur->next = parse_commands(ptr+1);
if (cur->next) {
cur->terminator = SEQUENCE;
}
break;
}
*ptr = 0;
ptr = skip_to_non_ws(ptr+1);
if (!ptr) {
break;
}
if (*ptr == SEQ_OP) {
/* found a sequence operator */
cur->next = parse_commands(ptr+1);
if (cur->next) {
cur->terminator = SEQUENCE;
}
break;
}
cur->arg[ix] = ptr;
++ix;
}
cur->arg[ix] = 0; cur->nargs = ix; return cur;
}


void   execute(struct cmd  *clist){
int    pid, npid, stat;

if(strcmp (clist->arg[0],"cd")==0) { /*Check if the command is cd*/

if (clist->arg[1] == NULL) { /*Check if the path is null*/
chdir(getenv("HOME")); /*Go back to home directory if null*/

}
else {
if (chdir(clist->arg[1]) == -1) { /*Take the path from the user*/
printf("%s: No such file or directory\n",clist-> arg[1]);
}
}
}


pid = fork();
if (pid == -1) {
perror("dumbshell:fork");
exit(1);
}

if (!pid) {
/* child */

if (strcmp (clist->arg[0] ,"cd")==0){ /*check if cd in the child process*/
exit(0);
}

if(strcmp (clist->arg[0],"pwd")==0) {
char cwd[700]; /*Size of the current working directory is 700*/
printf("Your current working directory is: %s\n", getcwd(cwd, 700));
}


exit(1);

}


do {
npid = wait(&stat);
printf("Process %d exited with status %d\n",npid,stat);
} while (npid != pid);
switch (clist->terminator) {
case SEQUENCE:
execute(clist->next);
}
}

void   free_commands(struct cmd   *clist)
{
struct cmd   *nxt;

do {
nxt = clist->next;
free(clist);
clist = nxt;
} while (clist);
}

char	*get_command(char   *buf, int	size, FILE	*in)
{

if (in == stdin) {
char cwd[700]; /*size of the current working directory is 700*/
/*get the current working directory and show it in the prompt*/
printf("%s@%s$ ", getenv("LOGNAME"), getcwd(cwd, 700)); /* prompt */
}
return fgets(buf,size,in);
}

void   main(void)
{
char         linebuf[MAXLINELEN];
struct cmd   *commands;

while (get_command(linebuf,MAXLINELEN,stdin) != NULL) {
commands = parse_commands(linebuf);
if (commands) {
execute(commands);
free_commands(commands);
}
}
}
