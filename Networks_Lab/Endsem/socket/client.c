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

void two_way_chat(int sockfd)
{
    char buff[MAX];

    read(sockfd, buff, sizeof(buff));
    printf("\tFrom Server : %s", buff);
    while(1)
    {
        bzero(buff, sizeof(buff));

        printf("Enter the string : ");
        fgets(buff,MAX,stdin);
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("\tFrom Server : %s\n", buff);

        if ((strncmp(buff, "exit",(int)4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}
int main()
{
    int sockfd, connfd;

    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
    two_way_chat(sockfd);
    close(sockfd);
}