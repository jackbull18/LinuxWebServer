#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void* mywork(void *arg){
    printf("child thread id :%ld\n", pthread_self());
    for(int i=0; i<5; i++){
        printf("child: %d\n",i);
    }
    return NULL;
}

int main(){
    //1、创建一个线程属性变量
    pthread_attr_t attr;
    //2、初始化属性变量
    pthread_attr_init(&attr);
    //3、设置属性--线程分离
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //创建一个子线程
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,mywork,NULL);
    if(ret != 0){
        char* errstr = strerror(ret);
        printf("error: %s\n",errstr);
    }
    //获取线程的栈的大小
    size_t size;
    pthread_attr_getstacksize(&attr, &size);
    printf("thread stack size: %ld\n",size);

    //输出主线程和子线程ID
    printf("tid: %ld, main thread id: %ld\n",tid, pthread_self());

    //释放线程资源
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);

    return 0;

}