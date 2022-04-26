#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {

char cwd[700]; // Size of the current working directory is 700
getcwd(cwd, cwd[700]); //It gets the current working directory
printf("Your current working directory is: %s\n", cwd);

}
