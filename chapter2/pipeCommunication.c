#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
//匿名管道实现父子进程间通信

int main(){
    //在创建子进程之前创建匿名管道
    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1){
        perror("pipe");
        exit(0);
    }
    
    //创建子进程
    pid_t pid = fork();
    if(pid > 0){
        //父进程
        printf("This is parent process, pid: %d\n", getpid());

        //通过管道读取数据
        char buf[1024] = {0};
        while (1)
        {
            int len = read(pipefd[0], buf, sizeof(buf));
            printf("parent recv: %s, len: %d\n", buf, getpid());
        }

    }else if(pid == 0){
        //子进程
        printf("This is child process, pid: %d\n", getpid());

        //通过管道发送数据
        char buf[1024] = {0};
        while (1){   
            char *str = "Hello I'm a child process";
            write(pipefd[1],str, strlen(str));
            sleep(1);
        }

    }


}