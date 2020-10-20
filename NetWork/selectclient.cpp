// NetWork.cpp: 定义应用程序的入口点。
//

#include "NetWork.h"
//#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
const int MESSAGE_LENGTH = 256;
int main()
{

    int client_sockfd;
    int len;
    struct sockaddr_in address;//服务器端网络地址结构体
     int result;
    char ch = 'A';
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立客户端socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(6666);
    len = sizeof(address);
    result = connect(client_sockfd, (struct sockaddr *)&address, len);
    if(result == -1)
    {
         perror("oops: client2");
         exit(1);
    }
    char buf[MESSAGE_LENGTH] = {0};
    do
    {
       printf("What message would you like to send?\n");
       memset(buf, 0, MESSAGE_LENGTH);
       std::cin >> buf;

       int len = write(client_sockfd, buf, strlen(buf));
        printf("write len:%d.\n", len);
       if(len <= 0)
       {
           if(len == 0)
           {
               perror("0 bytes written: ");
           }
           else
           {
               perror("server dead on write: ");
           }
        }
        else if(len != MESSAGE_LENGTH)
        {
           perror("incorrect amount written: ");
        }

        /* We expect a response */
        char response[MESSAGE_LENGTH] = {0};
        len = read(client_sockfd, response, MESSAGE_LENGTH);//wait until data rev

        if(len <= 0)
        {
           if(len == 0)
           {
               printf("connection closed.\n");
           }
           else
           {
               perror("server died when read expected: ");
           }
        }

        printf("read string from server,len=%d, = \"%s\"\n", len, response);
    }while(strcasecmp(buf, "q") != 0);

    close(client_sockfd);
    cout << "Hello client--" << endl;
    return 0;
}
