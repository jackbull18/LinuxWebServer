#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//有名管道进程间通信

int main(){
    //判断管道文件是否存在，不存在则创建
    int ret = access("FIFO",F_OK);
    if(ret == -1){
        printf("pipe is not exist\ncreate pipe");

        //创建管道文件
        ret = mkfifo("FIFO",0664);
        if(ret == -1){
            perror("mkfifo");
            exit(0);
        }
    }
    //以只写的方式打开管道
    int fd = open("FIFO",O_WRONLY);
    if(fd == -1){
        perror("open");
        exit(0);
    }

    //向管道写数据
    for (int i = 0; i < 10; i++)
    {
        char buf[1024] = {0};
        sprintf(buf,"hello, %d\n",i);
        printf("write data: %s\n",buf);
        write(fd,buf,strlen(buf));
        sleep(1);
    }
    close(fd);
    return 0;
}