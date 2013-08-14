#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 21
#define LENGTH 512 


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     
    int sockfd; 
    int nsockfd;
    char revbuf[LENGTH];                                                                //buffer for data from the server
    struct sockaddr_in ser_addr;                                                        // structure to hold server's address  
    {
        short int sin_family;                                                           //Address family
        unsigned short int sin_port;                                                    //Port number
        struct in_addr sin_addr;                                                        //Internet address
        unsigned char sin_zero[8];                                                      //Same size as struct sockaddr
    }
     
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)                               // Get the Socket file descriptor
    {
        fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
        exit(1);
    }

     
    ser_addr.sin_family = AF_INET;                                                       //set family to Internet
    ser_addr.sin_port = htons(PORT); 					                 //Host to Network Short (short, network byte order)
    inet_pton(AF_INET, "192.168.1.5", &ser_addr.sin_addr); 		       
    bzero(&(ser_addr.sin_zero), 8);                                                      //zero the rest of the struct


    
    if (connect(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr)) == -1)    //Connecting to server... or remote
    {
        fprintf(stderr, " Connect to the host Failed (errno = %d)\n",errno);
        exit(1);
    }
    else 
        printf("\n\nClient: Connected to server");

        system("pwd ; ls");					// Calling script to display contents in the current directory
    
     				//  File: "clienttransfer.txt" from Client being send to Server 
 
	printf("\n\nClient: File 'clienttransfer' to Server");  
        char* fs_name = "/home/sree/networking/clienttransfer.txt";		         
        char sdbuf[LENGTH]; 
        printf("\n\nClient: Sending file:%s to the Server... ", fs_name);
        FILE *fs = fopen(fs_name, "r");
        if(fs == NULL)
        {
            printf("ERROR: File %s not found.\n", fs_name);
            exit(1);
        }

        bzero(sdbuf, LENGTH); 
        int fs_block_sz; 
        while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
        {
            if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
            {
                fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
                break;
            }
            bzero(sdbuf, LENGTH);
        }
        printf("\n\n File: %s from Client was Sent!\n", fs_name);
    

    				//  File "servertransfer.txt" from Server being received at Client and saveing


    printf("\n\nClient: Receiveing file from Server...\n File saved as : 'fromserver.txt'");
    char* fr_name = "/home/sree/Desktop/fromserver.txt";
    FILE *fr = fopen(fr_name, "a");
    if(fr == NULL)
        printf("File %s Cannot be opened.\n", fr_name);
    else
    {
        bzero(revbuf, LENGTH); 
        int fr_block_sz = 0;
        while((fr_block_sz = recv(sockfd, revbuf, LENGTH, 0)) > 0)
        {
            int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
            if(write_sz < fr_block_sz)
            {
                error("\nFile write failed.\n");
            }
            bzero(revbuf, LENGTH);
            if (fr_block_sz == 0 || fr_block_sz != 512) 
            {
                break;
            }
        }
        if(fr_block_sz < 0)
        {
            if (errno == EAGAIN)
            {
                printf("\nClient:Timed out.\n");
            }
            else
            {
                fprintf(stderr, "recv() failed due to errno = %d\n", errno);
            }
        }
        printf("\n\nFile: %s received from server!\n",fr_name);
        fclose(fr);
    }


    close (sockfd);						                         //closeing the connection established from client side
    printf("\nClient: Connection lost.\n");
    return (0);
}
