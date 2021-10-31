#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define IP "127.0.0.1"
#define PORT_NUMBER 5000
#define MAXLEN 2048

int main(int argc, char **argv)
{
	int sockfd;
	char buffer[MAXLEN];
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT_NUMBER);
	servaddr.sin_addr.s_addr = inet_addr(IP);
	bzero(buffer, sizeof(buffer));
	FILE *f;
	char *filename;
	printf("Enter the filename to be sent!\n");
	scanf("%s", filename);
	f = fopen(filename, "r");
	if (f == NULL) // no file condition
	{
		printf("File not present\n");
		return -1;
	}
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)); // connect only if file is present
	int i;
	for (i = 0; i < MAXLEN; i++)
	{
		buffer[i] = fgetc(f);
		if (buffer[i] == EOF)
		{
			break;
		}
	}
	buffer[i] = '\0'; // to denote end of string as printf needs to know end of buffer
	write(sockfd, buffer, sizeof(buffer));
	printf("File %s is sent to server! \nFile Content:\n%s\n",filename,buffer);
	close(sockfd);

	return 0;
}
