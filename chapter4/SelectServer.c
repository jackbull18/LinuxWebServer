#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>
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

    //创建读检测集合
    //rSet 用于记录正在监听的集合， tmpSet用于记录在轮训过程中由内核态返回到用户态的集合
    fd_set rSet, tmpSet;
    //初始化集合
    FD_ZERO(&rSet);
    //添加监听文件描述符
    FD_SET(listenfd, &rSet);
    //当前最大的文件描述符为监听描述符
    int maxfd = listenfd;

    //不断循环等待客户端连接
    while (1){
        tmpSet = rSet;
        //使用select，设置为永久阻塞，有文件描述符变化才返回
        int num = select(maxfd+1, &tmpSet, NULL, NULL, NULL);
        if (num == -1){
            perror("select");
            exit(-1);
        }else if(num == 0){
            //当前无文件描述符变化，执行下一次遍历
            //在本次设置中无效，因为select被设置为阻塞，不会发送未改变的情况
            continue;
        }else{
            //首先判断监听文件描述符是否变化
            if(FD_ISSET(listenfd, &tmpSet)){
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

                //添加连接文件描述符到集合中
                FD_SET(connfd, &rSet);
                //更新最大文件描述符
                maxfd = maxfd > connfd ? maxfd : connfd;
            }
            //遍历集合判断是否有变化，有则通信
            char recv_buf[1024] = {0};
            char *send_buf = "hello I am Server!\n";
            for(int i = listenfd+1; i <= maxfd; i++) {
                if(FD_ISSET(i, &tmpSet)){
                    ret = read(i, recv_buf, sizeof(recv_buf));
                    if(ret == -1){
                        perror("read");
                        exit(-1);
                    }else if(ret > 0){
                        printf("recv server data: %s\n", recv_buf);
                        write(i, send_buf,strlen(send_buf));
                    }else{
                        //表示客户端断开连接
                        printf("client closed....\n");
                        close(i);
                        FD_CLR(i, &rSet);
                        break;
                    }
                }
            }
        }

    }
    close(listenfd);
    return 0;
    
}