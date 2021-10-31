#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#define IP "127.0.0.1"
#define PORT_NUMBER 5000
#define MAXLEN 2048

int main(int argc, char **argv)
{
	int sockfd, n;
	char buffer[MAXLEN];
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_port = htons(PORT_NUMBER);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(IP);
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while (1)
	{
		bzero(buffer, sizeof(buffer));
		printf("Enter your message (Type STOP to end communication with server)\n");
		fgets(buffer, MAXLEN, stdin);
		write(sockfd, buffer, sizeof(buffer));
		printf("Message sent to server\n");
		if ((strncmp("STOP", buffer, 4)) == 0)
		{
			printf("Communication end\n");
			break;
		}
	}
	close(sockfd);

	return 0;
}
