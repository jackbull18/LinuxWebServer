#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>


#define SERVERIP "127.0.0.1"
#define SERVERPORT 6789

int main(){
    //1、创建通信套接字
    int connfd = socket(AF_INET, SOCK_STREAM, 0);
    //2、绑定本机地址
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //点分十进制转换为网络字节序
    inet_pton(AF_INET, SERVERIP, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVERPORT);//主机字节序转为网络字节序
    int ret = bind( connfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    //3、通信
    while(1){

        char recv_buf[128];
        char ipbuf[16];

        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);

        //接受数据
        int num = recvfrom(connfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&cliaddr, &len);
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf));
        unsigned short client_port = ntohs(cliaddr.sin_port);
        printf("client IP : %s, Port : %d\n",ipbuf,client_port);
        printf("client say : %s\n", recv_buf);

        //发送数据
        sendto(connfd, recv_buf, strlen(recv_buf)+1, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

    }
    return 0;
}