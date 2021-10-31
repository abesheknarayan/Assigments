#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#define IP "127.0.0.1" // localhost ip
#define PORT_NUMBER 5000
#define MAXLEN 2048

int main()
{
	int sockfd, connfd, len;
	char buffer[MAXLEN];
	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr, sizeof(servaddr));
	bzero(&servaddr, sizeof(servaddr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT_NUMBER);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(sockfd, 2);
	len = sizeof(cliaddr);
	connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
	while (1)
	{
		bzero(buffer, MAXLEN);
		read(connfd, buffer, sizeof(buffer));
		printf("Message from client: %s\n", buffer);
		if (strncmp("STOP", buffer, 4) == 0)
		{
			printf("Communication end\n");
			break;
		}
	}
	close(sockfd);

	return 0;
}
