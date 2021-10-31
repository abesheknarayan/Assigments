#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX 2048
#define PORT 5000

void chat_2way_udp(int sockfd)
{
    char buff[MAX];
    int n, clen;
    struct sockaddr_in cli;
    clen = sizeof(cli);
    while(1)
    {
        bzero(buff, MAX);
        recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&cli, &clen);
        printf("From client %s\t To client: ", buff);
        bzero(buff, MAX);
        fgets(buff, MAX, stdin);
        sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&cli, clen);
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Server Exit...\n");
            break;
        }
    }
}
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
    chat_2way_udp(sockfd);
    close(sockfd);
}