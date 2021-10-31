#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <time.h>
#include <poll.h>

#define IP "127.0.0.1"
#define PORT 5000
#define MAX 2048

const int window_size = 4;

void send_frame(int sockfd, int packet)
{
    char buff[MAX];
    int expected_ack = packet % window_size; // 0/1 represents packet seq;
    printf("%d\n", packet);
    char message_head[] = "Packet ";
    int len1 = strlen(message_head);
    char packet_no[10];
    sprintf(packet_no, "%d", packet);
    int packet_log = strlen(packet_no);
    char message_foot[] = " is sent to server\n";
    int len2 = strlen(message_foot);

    // sending and recieving part
    bzero(buff, sizeof(buff));
    int index = 0;
    for (int i = 0; i < packet_log; i++)
        buff[index++] = packet_no[i];
    printf("%s ", message_head);
    printf("%s", buff);
    write(sockfd, buff, sizeof(buff));
    printf("%s \n", message_foot);
}

bool recv_ack(int sockfd, int packet)
{
    char buff[MAX];
    int expected_ack = packet % window_size; // 0/1 represents packet seq;
    bzero(buff, sizeof(buff));

    read(sockfd, buff, sizeof(buff));
    
    int received_ack = buff[0] - '0';
    // cout<<expected_ack<<" "<<received_ack<<"\n";
    // printf("%d ==== %d \n",expected_ack,received_ack);
    if (received_ack == expected_ack)
    {
        printf("Recieved positive acknowledgment from server\n");
        expected_ack = 1 - expected_ack;
        return true;
    }
    else
    {
        printf("Recieved negative acknowledgment from server\n");
        return false;
    }
}

void exit_chat(int sockfd)
{
    char buff[MAX];
    static int expected_ack = 1; // 0/1 represents packet seq;
    char exit[] = "EXIT";
    bzero(buff, sizeof(buff));

    for (int i = 0; i < 4; i++)
        buff[i] = exit[i];
    // sending and recieving part
    write(sockfd, buff, sizeof(buff));
    printf("\nExiting\n");
}



// void lost_package(int sockfd)
// {
//     int lost_package = rand()%8;

//     for (int i = 0; i < 2; i++)
//     {
//         while (1)
//         {
//             bool acks[window_size];
//             for (int j = 0; j < window_size; j++)
//             {
//                 acks[j] = true;
//             }
//             for (int j = 0; j < window_size; j++)
//             {
//                 int no = i* window_size +j;
//                 if(no==lost_package)
//                 {


//                     continue;
//                 }
//                 send_frame(sockfd, i * window_size + j );
//             }
//             for (int j = 0; j < window_size; j++)
//             {
//                 bool ack = recv_ack(sockfd, i * window_size + j);
//                 acks[j] = ack;
//             }
//             int no_wrong = 0;
//             for (int j = 0; j < window_size; j++)
//             {
//                 if (!acks[j])
//                 {
//                     no_wrong++;
//                 }
//             }
//             if (no_wrong == 0)
//             {
//                 break;
//             }
//         }
//         sleep(1);
//     }

//     // exit condition
//     exit_chat(sockfd);

// }

void normal_transmission(int sockfd)
{
    for (int i = 0; i < 2; i++)
    {
        while (1)
        {
            bool acks[window_size];
            for (int j = 0; j < window_size; j++)
            {
                acks[j] = true;
            }
            for (int j = 0; j < window_size; j++)
            {
                send_frame(sockfd, i * window_size + j );
            }
            for (int j = 0; j < window_size; j++)
            {
                bool ack = recv_ack(sockfd, i * window_size + j);
                acks[j] = ack;
            }
            int no_wrong = 0;
            for (int j = 0; j < window_size; j++)
            {
                if (!acks[j])
                {
                    no_wrong++;
                }
            }
            if (no_wrong == 0)
            {
                break;
            }
        }
        sleep(1);
    }

    // exit condition
    exit_chat(sockfd);
}


int main()
{
    int sockfd, connfd;
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
    inet_pton(AF_INET, IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    printf("Select a choice\n1.Normal Transmission\n2.Lost Package\n3.Damaged Ack\n4.Lost Ack\n");
    int choice;
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
    {
        normal_transmission(sockfd);
        break;
    }
    // case 2:
    // {
    //     lost_message(sockfd);
    // }
    default:
    {
        printf("please select a valid option\n");
        break;
    }
    }
    close(sockfd);
}