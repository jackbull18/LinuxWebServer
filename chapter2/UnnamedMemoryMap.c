#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(){
    //1、创建匿名内存映射区
    int len = 4096;
    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
    if(ptr== MAP_FAILED){
        perror("mmap");
        exit(0);
    }

    //2、父子间通信
    pid_t pid = fork();
    if(pid > 0){
        //父进程
        strcpy((char*) ptr, "Hello, this is parent");
        wait(NULL);//父进程被阻塞，只有当子进程执行完成，父进程才执行
    }else if(pid == 0){
        // 子进程
        sleep(1);
        printf("%s\n", (char *)ptr);
    }
    //3、释放内存映射区
    int ret = munmap(ptr,len);
    if(ret == -1){
        perror("munmap");
        exit(0);
    }
    return 0;
}