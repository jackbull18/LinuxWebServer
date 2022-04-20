/*
    TCP通信：客户端
*/
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 6789

int main(){
    //1、创建通信套接字
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    if(connfd == -1){
        perror("socket");
        exit(-1);
    }
    //2、连接服务器端
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    int ret = connect(connfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if(ret == -1){
        perror("connect");
        exit(-1);
    }
    //3、通信
    char recv_buf[1024] = {0};
    while (1){
        //发送数据
        char *send_buf = "client message";
        write(connfd, send_buf, strlen(send_buf));

        //接受数据
        ret = read(connfd, recv_buf, sizeof(recv_buf));
        if(ret == -1){
            perror("read");
            exit(-1);
        }else if(ret > 0){
            printf("recv server data : %s\n", recv_buf);
        }else{
            printf("server closed.....\n");
        }
        sleep(1);
    }
    close(connfd);
    return 0;
    
}
