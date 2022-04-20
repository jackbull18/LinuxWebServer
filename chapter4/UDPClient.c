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
    //2、通信
     //2、绑定本机地址
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //点分十进制转换为网络字节序
    inet_pton(AF_INET, SERVERIP, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVERPORT);//主机字节序转为网络字节序
    int num = 0;
    
    while (1){
        //发送数据
        char send_buf[128];
        sprintf(send_buf, "hello, this is client %d\n",num++);
        sendto(connfd, send_buf, sizeof(send_buf)+ 1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        //接受数据
        int num = recvfrom(connfd, send_buf, sizeof(send_buf), 0, NULL, NULL);
        printf("server say : %s\n", send_buf);
        sleep(1);
    }
    return 0;
}