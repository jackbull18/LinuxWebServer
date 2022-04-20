#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//有名管道进程间通信

int main(){
    
    //以只读的方式打开管道
    int fd = open("FIFO",O_RDONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    //从管道读数据
    while (1){
        char buf[1024] = {0};
        int len = read(fd,buf,sizeof(buf));
        if(len == 0){
            printf("The pipe is closed...\n");
            break;
        }
        printf("recv buf: %s",buf);
    }
    
    
    close(fd);
    return 0;
}