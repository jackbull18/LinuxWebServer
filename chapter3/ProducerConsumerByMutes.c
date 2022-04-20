/*
    使用条件变量实现的生产者消费者模型
    只能实现简单的线程同步，由于使用链表，缓存区无线，不存在无法生产的情况
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

//使用链表作为容器
struct Node{
    int val;
    struct Node* next;
};

//定义头节点
struct Node* head = NULL;

//定义互斥量
pthread_mutex_t mutex;



//生产者：使用头插法插入元素
void* producer(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));
        newnode->val = rand() % 1000;
        newnode->next = head;
        head = newnode;
        printf("add noden, num: %d, tid: %ld\n",newnode->val, pthread_self());

        
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    return NULL;
}

//消费者：使用头删法减少元素
void* consumer(void *arg){
    while (1){
        pthread_mutex_lock(&mutex);
        struct Node* tmp = head;
        if(head != NULL){
            head = head->next;
            printf("remove node,num: %d,tid: %ld\n",tmp->val,pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        }else{
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(){
    //初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    //创建5个生产者线程，和5个消费者线程
    pthread_t producers[5],consumers[5];
    for(int i = 0; i < 5; ++i){
        pthread_create(&producers[i],NULL, producer, NULL);
        pthread_create(&consumers[i],NULL, consumer, NULL);
    }
    //设置线程分离
    for (int i = 0; i < 5; i++)
    {
        pthread_detach(producers[i]);
        pthread_detach(consumers[i]);
    }
    //回收互斥锁、条件变量及主线程
    pthread_mutex_destroy(&mutex);
   
    pthread_exit(NULL);
    return 0;
}
