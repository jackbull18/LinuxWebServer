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
    //2、设置广播属性
    int op = 1;
    setsockopt(connfd, SOL_SOCKET, SO_BROADCAST, &op, sizeof(op));
    //3、创建一个广播地址
    struct sockaddr_in broad_addr;
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_port = htons(PORT);
    inet_pton(AF_INET,BROADCASTIP, &broad_addr.sin_addr.s_addr);

    //通信
    int num = 0;
    while(1){
        char send_buf[128];
        sprintf(send_buf, "Hello, client....%d", num++);
        sendto(connfd, send_buf, strlen(send_buf)+1, 0, (struct sockaddr *)&broad_addr, sizeof(broad_addr));
        printf("广播的数据： %s\n", send_buf);
        sleep(1);
    }
    close(connfd);
    return 0;
}