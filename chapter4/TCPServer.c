/*
    TCP通信:服务器端
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 6789

int main(){
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

    //4、接受客户端连接
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if(connfd == -1){
        perror("accept");
        exit(-1);
    }
    //输出客户端信息
    char client_ip[16] = {0};
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    unsigned short client_port = ntohs(client_addr.sin_port);
    printf("client ip: %s, client port: %d\n", client_ip, client_port);

    //5、开始通信
    char recv_buf[1024] = {0};
    while (1){
        //接受数据
        ret = read(connfd, recv_buf, sizeof(recv_buf));
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
        ret = write(connfd, send_buf,strlen(send_buf));
        if(ret == -1){
            perror("write");
            exit(-1);
        }
    }
    //关闭文件描述符
    close(listenfd);
    close(connfd);
    return 0;

}