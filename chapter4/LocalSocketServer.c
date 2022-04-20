#include <stdio.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    //本地套接字通信通过文件，使用前需要使用unlink解除文件占用
    unlink("server.sock");

    //1、创建socket
    int listenfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(listenfd == -1){
        perror("socket");
        exit(-1);
    }
    //2、绑定本地信息
    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path,"server.sock");
    int ret = bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    //3、监听
    ret = listen(listenfd, 8);
    if(ret == -1){
        perror("listen");
        exit(-1);
    }
    //4、接受客户端连接
    struct sockaddr_un client_addr;
    int len = sizeof(client_addr);
    client_addr.sun_family = AF_LOCAL;
    strcpy(client_addr.sun_path, "client.sock");
    int connfd = accept(listenfd, (struct sockaddr *)&client_addr,&len);
    if(connfd == -1){
        perror("accept");
        exit(-1);
    }
    //5、通信
    while(1){
        //接受信息
        char buf[1024];
        int buf_len = recv(connfd, buf, sizeof(buf), 0);
        if(buf_len == -1){
            perror("recv");
            exit(-1);
        }else if(buf_len == 0){
            printf("client close.....\n");
            break;
        }else{
            printf("server recv: %s\n", buf);
            send(connfd, buf, strlen(buf)+1, 0);
        }
    }
    close(connfd);
    close(listenfd);
    return 0;
}