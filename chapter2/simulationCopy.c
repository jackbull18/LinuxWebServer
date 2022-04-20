/*
    使用内存映射实现文本的拷贝功能
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    //打开源文件，获取文件长度并创建对应的内存映射区
    int fdSource = open("source.txt", O_RDONLY);
    int len = lseek(fdSource, 0, SEEK_END);
    void *ptrSource = mmap(NULL,len,PROT_READ, MAP_SHARED, fdSource,0);
    if(ptrSource == MAP_FAILED){
        perror("mmap1");
        exit(-1);
    }
    //打开目标文件,并创建对应的内存映射区
    int fdTarget = open("target.txt", O_RDWR | O_CREAT, 0664);
    //拓展文件长度
    truncate("target.txt",len);
    write(fdTarget," ",1);
    //创建内存映射区
     void *ptrTarget = mmap(NULL,len,PROT_READ | PROT_WRITE, MAP_SHARED, fdTarget,0);
    if(ptrTarget == MAP_FAILED){
        perror("mmap2");
        exit(-1);
    }
    //进行内存拷贝
    memcpy(ptrTarget,ptrSource,len);
    //回收资源
    close(fdTarget);
    close(fdSource);
    munmap(ptrTarget, len);
    munmap(ptrSource, len);

    return 0;

}