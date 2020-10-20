// NetWork.cpp: 定义应用程序的入口点。
//
//https://github.com/koponomarenko/networking

#include "NetWork.h"
//#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

size_t MSG_BUFFER_SIZE = 256;
int main()
{
   int server_sockfd, client_sockfd;
   socklen_t server_len, client_len;
   struct sockaddr_in server_address;
   struct sockaddr_in client_address;

   server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立服务器端socket
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(6666);
   server_len = sizeof(server_address);
   bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
   listen(server_sockfd, 2); //监听队列最多容纳2个


   fd_set readfds, testfds;
   FD_ZERO(&readfds);

   FD_SET(server_sockfd, &readfds);//将服务器端socket加入到集合中
   printf("server_sockfd:%d\n", server_sockfd);

   while(1)
   {
       char ch;
       int fd;
       int nread;
       testfds = readfds;//将需要监视的描述符集copy到select查询队列中，select会对其修改，所以一定要分开使用变量
       printf("server waiting, FD_SETSIZE:%d\n", FD_SETSIZE);

       /*无限期阻塞，并测试文件描述符变动 */
       int result = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0); //FD_SETSIZE：系统默认的最大文件描述符
       printf("server select end: %d\n", result);

       if(result < 1)
       {
           perror("server5");
           exit(1);
       }

       /*扫描所有的文件描述符*/
       for(fd = 0; fd < FD_SETSIZE; fd++)
       {
           /*找到相关文件描述符*/
           if(FD_ISSET(fd,&testfds))
           {

               printf("FD_ISSET: fd %d\n", fd);
             /*判断是否为服务器套接字，是则表示为客户请求连接。*/
               if(fd == server_sockfd)
               {
                   client_len = sizeof(client_address);
                   client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
                   FD_SET(client_sockfd, &readfds);//将客户端socket加入到集合中
                   printf("adding client on fd [%d]\n", client_sockfd);
               }
               /*客户端socket中有数据请求时*/
               else
               {
                    int rv;
                    char sent[MSG_BUFFER_SIZE];
                    memset(sent, 0, MSG_BUFFER_SIZE);

                    printf("[%d] begin recv\n", fd);
                    if ((rv = recv(fd, sent, MSG_BUFFER_SIZE, 0)) <= 0) {

                        if (rv != 0)
                            perror("error on recv call");
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("[%d] connection closed\n", fd);
                        continue;
                    }

                    // print and echo
                    printf("[%d] \"%s\"\n", fd, sent);
                    char reply[MSG_BUFFER_SIZE];
                    sprintf(reply, "you sent \"%s\"", sent);

                    rv = send(fd, reply, strlen(reply), 0);
                    if (rv < 0) {
                        perror("error on send call");
                    }


               }
           }

       }
   }



    cout << "Hello CMake。--" << endl;
    return 0;
}
