#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define SERV_PORT 5000
#define MAXLINE 1024 
char *END_FLAG = "\0";

void run(int sockfd, struct sockaddr *cliaddr, socklen_t clilen)
{
    socklen_t len;
    char buf[MAXLINE];
    len = clilen;
    FILE *fp;
    int n=recvfrom(sockfd, (char *)buf, MAXLINE,MSG_WAITALL, (struct sockaddr *)&cliaddr,&len);
    buf[n]='\0';
    fp = fopen("server.txt", "w");
	fprintf(fp, "%s", buf);
    fclose(fp);
}
int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    run(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
    printf("File received successfully\n");
    return 0;
}