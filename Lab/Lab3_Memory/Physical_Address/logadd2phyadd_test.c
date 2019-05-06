#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 4096
char buf[BUFSIZE];

typedef struct data
{
    unsigned long addr;
    int p;
} mydata;

int main()
{
    unsigned long tmp, addr;
    int fd, len;
    mydata wdata;
    tmp = 0x12345678;
    addr = &tmp;
    //Output tmp?
    //Output addr?
    wdata.addr = addr;
    wdata.p = getpid();
    //Output Process ID pid?
    //Open File
    fd = open("/proc/logadd2phyadd", O_RDWR);
    //Write wdata into file?
    //Read file?
    //Output file length and content
    printf("the read length is %d and the buf content is: \n%s\n", len, buf);

    sleep(2);
    close(fd);
    return 0;
}
