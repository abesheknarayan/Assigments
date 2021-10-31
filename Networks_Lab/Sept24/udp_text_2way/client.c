#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include<stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>


#define MAX 2048
#define PORT 5000


int main()
{
    char buff[MAX];
    int sockfd, len, n;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(len));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    len = sizeof(servaddr);
    while(1)
    {
        printf("\nEnter string : ");
        fgets(buff,MAX,stdin);
        sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, len);
        bzero(buff, sizeof(buff));
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len);
        printf("From Server : %s\n", buff);
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
    close(sockfd);
}