#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){

    pid_t pid = fork();

    if(pid < 0){
        printf("This is parent process, pid: %d\n", getpid());
    }else if(pid == 0){
        printf("This is child process, pid: %d\n", getpid());
        //调用exec函数执行
        execlp("ps", "ps", "aux", NULL);//NUll结束参数列表
    }
    for (int i = 0; i < 3; i++)
    {
        printf("i = %d, pid = %d\n", i, getpid());
        sleep(1);
    }
    return 0;
    
    return 0;
}
