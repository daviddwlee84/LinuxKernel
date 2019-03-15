#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {

    int a = 87;
    int pid;

    printf("Result of vfork()\n");
    printf("a before vfork is %d\n", a);

    pid = vfork();
    if(pid == 0) { // Child
        a += 3;
        exit(0);
    }
    // Parent
    wait(NULL);
    printf("a after vfork is %d\n", a);
    exit(0);
}