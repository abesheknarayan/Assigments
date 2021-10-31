#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <poll.h>

#define IP "127.0.0.1"
#define PORT 5000
#define MAX 2048

const int window_size = 4;

bool recv_frame(int sockfd, int damage_ack)
{
    static int expected_packet_seq = 0; // 0/1 based on packet number %2
    static int expected_packet_number = 0;
    printf("expecting packet %d\n", expected_packet_number);
    char buff[MAX];
    while (1)
    {
        bzero(buff, MAX);
        read(sockfd, buff, sizeof(buff));
        int len = strlen(buff);
        if (strcmp("EXIT", buff) == 0)
        {
            return false;
        }
        int packet_number = (buff[len - 1] - '0');
        int temp_packet_number = packet_number;
        packet_number %= window_size;
        // cout<<packet_number<<"==========\n";
        printf("%d==============",packet_number);
        if (packet_number == expected_packet_seq)
        {
            // send + ack
            printf("From client: recived packet %s succesfully\nSending ACK %d\n", buff, packet_number);
            bzero(buff, MAX);
            int index = 0;
            buff[0] = (char)('0' + (expected_packet_seq));
            expected_packet_number++;
            expected_packet_number %=window_size;
            expected_packet_seq++;
            expected_packet_seq%=window_size;
            return true;
        }
        // else
        // {
        //     // send - ack
        //     printf("From Client: Recieved Damaged package!!\n Sending Negative ACK\n");
        //     bzero(buff, MAX);
        //     int index = 0;
        //     buff[0] = (char)('0' +(expected_packet_seq));
        //     return false;
        // }
    }
}

void send_ack(int sockfd, int packet, int ack)
{
    char buff[MAX];
    bzero(buff, MAX);
    
    buff[0] = (char)('0' + (packet%window_size));
    write(sockfd, buff, sizeof(buff));
}

void normal_transmission(int sockfd)
{
    int frames = 0;
    int i, j;
    for (i = 0; i < 2; i++)
    {
        while (1)
        {
            bool acks[window_size];
            for (j = 0; j < window_size; j++)
            {
                acks[j] = true;
            }
            for (j = 0; j < window_size; j++)
            {
                bool ack = recv_frame(sockfd, -1);
                acks[j] = ack;
            }
            int no_wrong = 0;
            for (j = 0; j < window_size; j++)
            {
                send_ack(sockfd, j,acks[j]);
                if (acks[j] == 0)
                {
                    no_wrong++;
                }
            }
            if (no_wrong == 0)
            {
                break;
            }
        }
    }
    printf("Exiting\n");
}

int main()
{
    int sockfd, connfd, len;
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
    connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
    if (connfd < 0)
    {
        printf("server acccept failed...\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");

    printf("Select a choice\n1.Normal Transmission\n2.Lost Package\n3.Damaged Ack\n4.Lost Ack\n");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
    {
        normal_transmission(connfd);
        break;
    }
    default:
    {
        printf("please select a valid option\n");
        break;
    }
    }
    close(sockfd);
}