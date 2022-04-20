#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    //创建子进程
    pid_t pid = fork();
    /*
        pid_t fork(void);
        函数的作用：用于创建子进程。
        返回值：
            fork()的返回值会返回两次。一次是在父进程中，一次是在子进程中。
            在父进程中返回创建的子进程的ID,
            在子进程中返回0
            如何区分父进程和子进程：通过fork的返回值。
            在父进程中返回-1，表示创建子进程失败，并且设置errno
    */
   //通过pid的值判断当前进程是否为子进程
   if(pid > 0){
       //这是父进程
       printf("pid: %d\n", pid);
       printf("This is parent process, pid: %d, ppid: %d\n", getpid(), getppid());
   }else if(pid == 0){
       //这里是子进程
       printf("pid: %d\n", pid);
       printf("This is child process, pid: %d, ppid: %d\n", getpid(), getppid());
   }else{
       perror("fork");
       exit(-1);
   }
   //进程的实际工作代码
   for (int i = 0; i < 3; i++)
   {
       printf("i : %d, pid : %d\n", i, getpid());
       sleep(1);
   }
   return 0; 
}