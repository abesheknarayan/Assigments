#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define SERV_PORT 5000
#define MAXLINE 2048
char *END_FLAG = "\0";

int main(int argc, char **argv)
{
    int sockfd, n, fd;
    struct sockaddr_in servaddr;
    char buf[MAXLINE];
    char *target, *path;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    FILE *f;
    char *filename;

    f=fopen("client.txt","r");
    if (f == NULL)
    {
        printf("File not present\n");
        return -1;
    }
    int i;
    for (i = 0; i < MAXLINE; i++)
    {
        buf[i] = fgetc(f);
        if (buf[i] == EOF)
        {
            break;
        }
    }
    buf[i] = '\0';
    sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("File sent to the server\n");
    return 0;
}