#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {

    int a = 87;
    int pid;

    printf("Result of fork()\n");
    printf("a before fork is %d\n", a);

    pid = fork();
    if(pid == 0) { // Child
        a += 3;
        exit(0);
    }
    // Parent
    wait(NULL);
    printf("a after fork is %d\n", a);
    exit(0);
}