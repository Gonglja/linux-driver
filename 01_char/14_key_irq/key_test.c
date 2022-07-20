#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
    char path[] = "/dev/key";
    int fd = open(path, O_RDWR);
    char data[1]={0};
    int ret;

    if(fd < 0) {
        printf("open %s failed.\r\n", path);
    }
    printf("open %s success.\r\n", path);

    while(1) {
        ret = read(fd, data, 1);
        if(ret < 0) { /* 读取错误或者没有值 */
        } else {
            if(data[0]) {
                printf("key value = %#x\r\n", data[0]);
            }
        }
    }
    close(fd);
    return 0;
} 