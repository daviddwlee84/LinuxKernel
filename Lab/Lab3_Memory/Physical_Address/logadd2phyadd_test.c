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
    int pid;
} mydata;

int main()
{
    unsigned long tmp, addr;
    int fd, len;
    mydata wdata;
    tmp = 0x12345678;
    addr = &tmp;
    printf("tmp value is: %lX\n", tmp);
    printf("tmp address is: %lX\n", addr);
    wdata.addr = addr;
    wdata.pid = getpid();
    printf("the pid is: %d\n", wdata.pid);
    fd = open("/proc/logadd2phyadd", O_RDWR);
    if (fd == -1)
    {
        printf("Fail to open file /proc/logadd2phyadd");
        return 1;
    }
    write(fd, &wdata, sizeof(mydata));
    len = read(fd, buf, BUFSIZE);
    //Output file length and content
    printf("the read length is %d and the buf content is: \n%s\n", len, buf);

    sleep(2);
    close(fd);
    return 0;
}
