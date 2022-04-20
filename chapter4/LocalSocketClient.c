#include <stdio.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
    //本地套接字通信通过文件，使用前需要使用unlink解除文件占用
    unlink("client.sock");

    //1、创建socket
    int connfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(connfd == -1){
        perror("socket");
        exit(-1);
    }
    //2、绑定本地信息
    struct sockaddr_un client_addr;
    client_addr.sun_family = AF_LOCAL;
    strcpy(client_addr.sun_path,"client.sock");
    int ret = bind(connfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if(ret == -1){
        perror("bind");
        exit(-1);
    }
    //4、接受客户端连接
    struct sockaddr_un server_addr;
    int len = sizeof(server_addr);
    server_addr.sun_family = AF_LOCAL;
    strcpy(server_addr.sun_path, "server.sock");
    ret = connect(connfd, (struct sockaddr *)&server_addr,len);
    if(ret == -1){
        perror("connect");
        exit(-1);
    }
    //5、通信
    int num = 0;
    while(1){
        //发送数据
        char buf[1024];
        sprintf(buf, "the data is %d\n", num++);
        send(connfd, buf, strlen(buf)+1, 0);
        //接受信息
        int buf_len = recv(connfd, buf, sizeof(buf), 0);
        if(buf_len == -1){
            perror("recv");
            exit(-1);
        }else if(buf_len == 0){
            printf("server close.....\n");
            break;
        }else{
            printf("client recv: %s\n", buf);
        }
        sleep(1);
    }
    close(connfd);
    return 0;
}