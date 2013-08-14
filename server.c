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

#define PORT 20000
#define BACKLOG 5
#define LENGTH 512 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main ()
{
    
    int sockfd; 
    int nsockfd; 
    int num;
    int sin_size; 
    struct sockaddr_in cli_adr; 									// structure to hold client's address
    struct sockaddr_in ser_adr; 									// structure to hold server's address
    char revbuf[LENGTH]; 										// Receiver buffer

    						 
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )						//The Socket file descriptor
    {
        fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("\nServer:Obtaining socket descriptor successfully.\n");

    
    cli_adr.sin_family = AF_INET; 									// Protocol Family
    cli_adr.sin_port = htons(PORT); 									// Host to Network Short 
    cli_adr.sin_addr.s_addr = INADDR_ANY;								// AutoFill local address
    bzero(&(cli_adr.sin_zero), 8);									// Flush the rest of struct

                                            // Binding to a special Port 

    if( bind(sockfd, (struct sockaddr*)&cli_adr, sizeof(struct sockaddr)) == -1 )
    {
        fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
        exit(1);
    }
    else 
        printf("\nServer: Binded tcp port sucessfully.\n");

                                      // Listen remote or server connect/calling 

    if(listen(sockfd,BACKLOG) == -1)
    {
        fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
        exit(1);
    }
    else
        printf ("\nServer: Listening to the port successfully.\n");

    int success = 0;
    while(success == 0)
    {
        sin_size = sizeof(struct sockaddr_in);

                          // Waiting for a connection, and obtain a new socket file despriptor for single connection 

        if ((nsockfd = accept(sockfd, (struct sockaddr *)&ser_adr, &sin_size)) == -1) 
        {
            fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
            exit(1);
        }
        else 
            printf("\nServer: Server has got connected from %s.\n", inet_ntoa(ser_adr.sin_addr));

  
	     system("pwd ; ls");		                // Calling script to display contents in the current directory

		
			        // File "clienttransfer.txt" from Client receiving at the server 

	printf("\nServer: Receiveing file from client...\n\n File Saved as : 'fromclient.txt'");
        char* fr_name = "/home/sree/Desktop/fromclient.txt";
        FILE *fr = fopen(fr_name, "a");
        if(fr == NULL)
            printf("\nFile %s Cannot be opened file on server.\n", fr_name);
        else
        {
            bzero(revbuf, LENGTH); 
            int fr_block_sz = 0;
            while((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
            {
                int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
                if(write_sz < fr_block_sz)
                {
                    error("\nFile write failed on server.\n");
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
                    printf("Timed out.\n");
                }
                else
                {
                    fprintf(stderr, "recv() failed due to errno = %d\n", errno);
                    exit(1);
                }
            }
            printf("\nServer: File :%s received from client!\n",fr_name);
            fclose(fr); 
        }

        
        			// File 'servertransfer.txt' from server being send to Client 
 

           printf("\n\nServer: File 'servertransfer' to client");  
	    char* fs_name = "/home/sree/networking/nw/servertransfer.txt";
            char sdbuf[LENGTH]; 							// Send buffer
            printf("\n\nServer: Sending %s to the Client", fs_name);
            FILE *fs = fopen(fs_name, "r");
            if(fs == NULL)
            {
                fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
                exit(1);
            }

            bzero(sdbuf, LENGTH); 
            int fs_block_sz; 
            while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0)
            {
                if(send(nsockfd, sdbuf, fs_block_sz, 0) < 0)
                {
                    fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
                    exit(1);
                }
                bzero(sdbuf, LENGTH);
            }
            printf("\n\nServer: File: %s sent to client!\n",fs_name);
            success = 1;

            close(nsockfd);								//closeing the connection established from SERVER side
            printf("Server : Connection with Client closed.\n");
            while(waitpid(-1, NULL, WNOHANG) > 0);
        
    }
}
