#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 5000
#define MAX 2048

void one_way_message(int sockfd,int client_no)
{
    char buff[MAX];
    while (1)
    {
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        int len = strlen(buff);
        if(len==0)break;
        printf("Client %d recieved : %s\n",client_no, buff);
        break;
    }
}
int main()
{
    int sockfd[10], connfd;
    struct sockaddr_in servaddr, cli;
    for (int i = 0; i < 10; i++)
    {
        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd[i] == -1)
        {
            printf("socket creation failed...\n");
            exit(0);
        }
        else
            printf("Socket successfully created..\n");
    }

    // create 10 sockets with different ip
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT);
    for (int i = 0; i < 10; i++)
    {
        if (connect(sockfd[i], (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
        {
            printf("connection with the server failed...\n");
            exit(0);
        }
        else
            printf("socket %d connected to the server..\n", i + 1);
    }
    for(int i=0;i<10;i++)
    {
        one_way_message(sockfd[i],i+1);
    }
    for (int i = 0; i < 10; i++)
    {
        close(sockfd[i]);
    }
}