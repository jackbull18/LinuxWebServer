/*
    TCP通信:服务器端
    多进程并发通信
    父进程负责等待并接受客户端连接
    子进程完成通信
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 6789

struct sockInfo {
    int fd;
    pthread_t tid;
    struct sockaddr_in addr;
};
//创建线程数组
struct sockInfo sockinfos[128];

//线程工作函数
void* working(void *arg){
    //获得通信信息
    struct sockInfo *info = (struct sockInfo*) arg;
    //通信
    //输出客户端信息
    char client_ip[16] = {0};
    inet_ntop(AF_INET, &info->addr.sin_addr, client_ip, sizeof(client_ip));
    unsigned short client_port = ntohs(info->addr.sin_port);
    printf("client ip: %s, client port: %d\n", client_ip, client_port);

    //5、开始通信
    char recv_buf[1024] = {0};
    while (1){
        //接受数据
        int ret = read(info->fd, recv_buf, sizeof(recv_buf));
        if(ret == -1){
            perror("read");
            exit(-1);
        }else if(ret > 0){
            printf("recv client data : %s\n", recv_buf);
        }else{
            printf("client closed...\n");
            break;
        }
        //发送数据
        char *send_buf = "hello I am Server!\n";
        ret = write(info->fd, send_buf,strlen(send_buf));
        if(ret == -1){
            perror("write");
            exit(-1);
        }
    }
    close(info->fd);
}

int main(){
   
    //初始化线程结构体数组
    int sockinfo_maxlen = sizeof(sockinfos) / sizeof(sockinfos[0]);
    for (int i = 0; i < sockinfo_maxlen; i++){
        bzero(&sockinfos[i], sizeof(sockinfos[i]));
        sockinfos[i].fd = -1;
        sockinfos[i].tid = -1;
    }

    //1、创建socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if( listenfd == -1 ){
        perror("socket");
        exit(-1);
    }

    //2、绑定
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //点分十进制转换为网络字节序
    inet_pton(AF_INET, SERVERIP, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVERPORT);//主机字节序转为网络字节序
    int ret = bind( listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    //3、监听
    ret = listen( listenfd, 8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }
    while(1){
        //4、接受客户端连接
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if(connfd == -1){
            perror("accept");
            exit(-1);
        }
        //创建子线程处理
        struct sockInfo *pinfo;
        //从线程数组中找到可用的元素，进行赋值
        for (int i = 0; i < sockinfo_maxlen; i++)
        {
            if(sockinfos[i].tid == -1){
                pinfo = &sockinfos[i];
                break;
            }
            // 当遍历到最后还每找到时，休眠1秒后继续找
            if(i == sockinfo_maxlen -1){
                sleep(1);
                i = -1;
            }
        }
        //结构体赋值
        pinfo->fd = connfd;
        memcpy(&pinfo->addr, &client_addr, client_addr_len);
        pthread_create(&pinfo->tid, NULL, working, pinfo);

        //线程分离
        pthread_detach(pinfo->tid);
    }
    //关闭文件描述符
    close(listenfd);
    
    return 0;

}