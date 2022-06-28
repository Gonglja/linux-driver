#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


/*
 * ./led_test -w 0
 * ./led_test -r
 */
int main(int argc, char **argv) {
    char buf[1024];
    int fd;
    int ret;
    int len;

    /* 1. 判断参数 */
	if (argc < 2) {
		printf("Usage: %s -w <string>\n", argv[0]);
		printf("       %s -r\n", argv[0]);
		return -1;
	}

    
    /* 2. 打开文件 */
    fd = open("/dev/led", O_RDWR);
    if(fd == -1) {
        printf("can not open file /dev/led\n");
        return -1;
    }
    printf("open file /dev/led ok\n");

    /* 3. 写文件或读文件 */
    if ((0 == strcmp(argv[1], "-w")) && (argc == 3))
    {
        len = strlen(argv[2]) + 1;
        len = 1;
        char status[1];
        status[0] = strtol(argv[2], status, 16);
        printf("status:%d\r\n",status[0]);
        ret = write(fd, status, len);
    }
    else
    {
        len = read(fd, buf, 1024);		
        printf("APP read size: %d\n", len);
        buf[1023] = '\0';
        printf("APP read buff: %s\n", buf);
    }
    
    close(fd);

    return 0;
} 