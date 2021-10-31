#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define IP "127.0.0.1"
#define PORT 5000
#define MAX 2048

char *message = "Sending info from server";

void cast(int sockfd, char *mess,int client_no)
{

    if (strcmp("EXIT", mess) == 0)
    {
        printf("Exiting\n");
        return;
    }
    char buff[MAX];

    bzero(buff, MAX);
    int len = strlen(mess);

    for (int i = 0; i < len; i++)
    {
        buff[i] = mess[i];
    }
    printf("Sent message to client id : %d\n",client_no);
    write(sockfd, buff, sizeof(buff));
    
}

void exit_cast(int *connfd)
{
    cast(-1,"EXIT",-1);   
}

void broadcast(int *connfd)
{
    // send to all clients
    // printf("Called1\n");
    for (int i = 0; i < 10; i++)
    {
        cast(connfd[i], message,i+1);
    }
    exit_cast(connfd);
}

void multicast(int *connfd)
{
    // select some client and send
    // printf("Called2\n");
    int send_ind[100];
    for (int i = 0; i < 100; i++)
    {
        send_ind[i] = -1;
    }
    printf("Select indexes of clients\nPress -1 to break\n");
    while (1)
    {
        int index;
        scanf("%d", &index);
        if (index == -1)
            break;
        cast(connfd[index - 1], message,index);
    }
    exit_cast(connfd);
}

int main()
{
    int sockfd, connfd[10], len;
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
    if ((listen(sockfd, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
    for (int i = 0; i < 10; i++)
    {
        connfd[i] = accept(sockfd, (struct sockaddr *)&cli, &len);
        if (connfd[i] < 0)
        {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
            printf("server acccept the client...\n");
    }
    // two_way_chat(connfd);
    int choice;
    printf("1.Broadcast\n2.Multicast\n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
    {
        broadcast(connfd);
        break;
    }
    case 2:
    {
        multicast(connfd);
        break;
    }
    default:
    {
        break;
    }
    }

    close(sockfd);
}