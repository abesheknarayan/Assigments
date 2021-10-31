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

const window_size = 4;

bool two_way_chat(int sockfd, int damage_ack, bool delay)
{
    static int expected_packet_seq = 1; // 0/1 based on packet number %2
    static int expected_packet_number = 1;
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
        packet_number %= 2;
        if (packet_number == expected_packet_seq)
        {
            if (temp_packet_number == damage_ack)
            {
                    // forcefully send negative ack
                    printf("From client: recived packet %s succesfully\nSending damaged ACK %d\n", buff, packet_number);
                    bzero(buff, MAX);
                    int index = 0;
                    buff[0] = (char)('0' + 1 - (expected_packet_seq));
                    write(sockfd, buff, sizeof(buff));
                    return true;
                
            }
            else
            {
                // send + ack
                printf("From client: recived packet %s succesfully\nSending ACK %d\n", buff, packet_number);
                bzero(buff, MAX);
                int index = 0;
                buff[0] = (char)('0' + (expected_packet_seq));
                write(sockfd, buff, sizeof(buff));
                expected_packet_seq = 1 - expected_packet_seq;
                expected_packet_number++;
                return true;
            }
        }
        else
        {
            // send - ack
            printf("From Client: Recieved Damaged package!!\n Sending Negative ACK\n");
            bzero(buff, MAX);
            int index = 0;
            buff[0] = (char)('0' + 1 - (expected_packet_seq));
            write(sockfd, buff, sizeof(buff));
            return true;
        }
    }
}

void normal_transmission(int sockfd)
{
    while (two_way_chat(sockfd, -1, false) != 0)
    {
    }
    printf("Exiting\n");
}

void damaged_pkg_transmission(int sockfd)
{
    while (two_way_chat(sockfd, -1, false) != 0)
    {
    }
    printf("Exiting\n");
}

void damaged_ack_transmission(int sockfd)
{
    srand(time(0));
    int rand_no = (rand() % 9);
    int sent_packets = 0;
    while (two_way_chat(sockfd, rand_no, false) != 0)
    {
        sent_packets++;
        if (sent_packets == rand_no)
        {
            rand_no = -1;
        }
    }
    printf("Exiting\n");
}

void lost_ack_transmission(int sockfd)
{
    // generate random no and for that packet dont send ack
    srand(time(0));
    int rand_no = (rand() % 9);
    int sent_packets = 0;
    while (two_way_chat(sockfd, rand_no, (sent_packets + 1) == rand_no) != 0)
    {
        sent_packets++;
        if (sent_packets == rand_no)
        {
            rand_no = -1;
        }
    }
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
    case 2:
    {
        damaged_pkg_transmission(connfd);
        break;
    }
    case 3:
    {
        damaged_ack_transmission(connfd);
        break;
    }
    case 4:
    {
        lost_ack_transmission(connfd);
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