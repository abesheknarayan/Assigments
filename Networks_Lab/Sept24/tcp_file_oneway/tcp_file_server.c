#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define IP "127.0.0.1"
#define PORT_NUMBER 5000
#define MAXLEN 2048

int main()
{
	int sockfd, connfd, len;
	char buffer[MAXLEN];
	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr, sizeof(servaddr));
	bzero(&cliaddr, sizeof(cliaddr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT_NUMBER);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(sockfd, 2);

	len = sizeof(cliaddr);
	connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
	read(connfd, buffer, MAXLEN);
	printf("Recieved File Content:\n%s\n", buffer);
	FILE *fp;
	// writing content to output.txt , File output.txt will be automatically created if not present in the directory
	fp = fopen("output.txt", "w");
	fprintf(fp, "%s", buffer);
	printf("File received successfully\n");
	close(sockfd);
	return 0;
}
