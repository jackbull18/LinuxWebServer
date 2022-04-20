#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BROADCASTIP "192.168.213.255"
#define PORT 6789

int main(){
    //1、创建socket
    int connfd = socket(AF_INET, SOCK_DGRAM, 0);

    //3、绑定通信地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET,BROADCASTIP, &addr.sin_addr.s_addr);
    //addr.sin_addr.s_addr = INADDR_ANY;


    int ret = bind(connfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }

    while(1){
        char buf[128];
        int num = recvfrom(connfd, buf, sizeof(buf), 0, NULL, NULL);
        printf("recv:  %s\n", buf);
    }
    close(connfd);
    return 0;
}