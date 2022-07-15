#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <sys/ioctl.h>


#define CLOSE_CMD       (_IO(0XEF, 0x1)) /* 关闭定时器 */
#define OPEN_CMD        (_IO(0XEF, 0x2)) /* 打开定时器 */
#define SETPERIOD_CMD   (_IO(0XEF, 0x3)) /* 设置定时器周期命令 */

/*
 * ./led_test 
 */
int main(int argc, char **argv) {
    char buf[1024];
    int fd;
    int ret;
    int len;
    unsigned int cmd;
    unsigned int arg;
    unsigned char str[100];
    
    /* 2. 打开文件 */
    fd = open("/dev/led", O_RDWR);
    if(fd == -1) {
        printf("can not open file /dev/led\n");
        return -1;
    }
    printf("open file /dev/led ok\n");

    
    while(1) {
        printf("Input CMD:");
        ret = scanf("%d",&cmd);
        if( ret != -1) {
            fgets(str, 100, stdin);
        }

        switch (cmd)
        {
        case 1:
            ioctl(fd, CLOSE_CMD, arg);
            break;
        case 2:
            ioctl(fd, OPEN_CMD, arg);
            break;
        case 3:
            printf("Input Timer Period:");
            ret = scanf("%d",&arg);
            if( ret != -1) {
                fgets(str, 100, stdin);
            }
            ioctl(fd, SETPERIOD_CMD, arg);
            break;
        
        default:
            break;
        }
        usleep(100);
    }

    close(fd);

    return 0;
} 