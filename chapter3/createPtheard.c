#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void* mywork(void* arg){
    printf("child thread...\n");
    printf("num = %d\n",*(int*)arg);
    return NULL;
}

int main(){
    //创建子线程
    pthread_t tid;
    int num = 10;
    int ret = pthread_create(&tid, NULL,mywork,(void*)&num);
    if(ret != 0){
        char* errstr = strerror(ret);
        printf("error : %s\n",errstr);
    }
    printf("main thread,child thread ID: %d\n",ret);
    sleep(1);
    return 0;
}