
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#define closesocket close
#define PROTOPORT         5193          /* default protocol port number */
#define QLEN              6             /* size of request queue        */

int visits =  0;                        /* counts client connections     */
/*----------------------------------------------------------------------------
* Program:      server
*
* Purpose:        allocate a socket and then repeatedly execute the folllowing:
*                          (1) wait for the next connection from a client
*                          (2) send a short message to the client
*                          (3) close the connection
*                          (4) go back to step (1)
*
* Syntax:          server [ port ]
*
*                            port  - protocol port number to use
*
* Note:              The port argument is optional. If no port is specified,
*                        the server uses the default given by PROTOPORT.
*
*----------------------------------------------------------------------------
*/
main (argc, argv)
int           argc;
char         *argv[];
{
     struct   hostent   *ptrh;     /* pointer to a host table entry */
     struct   protoent  *ptrp;     /* pointer to a protocol table entry */
     struct   sockaddr_in sad;     /* structure to hold server's address */
     struct   sockaddr_in cad;     /* structure to hold client's address */
     int      sd, sd2;             /* socket descriptors */
     int      port;                /* protocol port number */
     int      alen;                /* length of address */
     char     buf[1000];           /* buffer for string the server sends */

//#ifdef  WIN32
//#define WSADATA wsaData;
//#define WSAstartup (0x0101, &wsaData);
//#endif
     memset((char  *)&sad,0,sizeof(sad)); /* clear sockaddr structure   */
     sad.sin_family = AF_INET;            /* set family to Internet     */
     sad.sin_addr.s_addr = INADDR_ANY;    /* set the local IP address */

     /* Check  command-line argument for protocol port and extract      */
     /* port number if one is specfied.  Otherwise, use the default     */
     /* port value given by constant PROTOPORT                          */
    
     if (argc > 1) {                        /* if argument specified     */
                     port = atoi (argv[1]); /* convert argument to binary*/
     } else {
                      port = PROTOPORT;     /* use default port number   */
     }
     if (port > 0)                          /* test for illegal value    */
                      sad.sin_port = htons((u_short)port);
     else {                                /* print error message and exit */
                      fprintf (stderr, "bad port number %s/n",argv[1]);
                      exit (1);
     }

     /* Map TCP transport protocol name to protocol number */
     
     if ( ((int)(ptrp = getprotobyname("tcp"))) == 0)  {
                     fprintf(stderr, "cannot map \"tcp\" to protocol number");
                     exit (1);
     }

     /* Create a socket */
     sd = socket (PF_INET, SOCK_STREAM, ptrp->p_proto);
     if (sd < 0) {
                       fprintf(stderr, "socket creation failed\n");
                       exit(1);
     }

     /* Bind a local address to the socket */
     if (bind(sd, (struct sockaddr *)&sad, sizeof (sad)) < 0) {
                        fprintf(stderr,"bind failed\n");
                        exit(1);
     }

     /* Specify a size of request queue */
     if (listen(sd, QLEN) < 0) {
                        fprintf(stderr,"listen failed\n");
                         exit(1);
     }

     /* Main server loop - accept and handle requests */
     fprintf( stderr, "Server up and running.\n");
     while (1) {
         alen = sizeof(cad);
         fprintf( stderr, "SERVER: Waiting for contact ...\n");
         
         if (  (sd2=accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
                              fprintf(stderr, "accept failed\n");
                              exit (1);
           }
           visits++;
           sprintf(buf,"This server has been contacted %d time%s\n",
                        visits, visits==1?".":"s.");
           fprintf( stderr, "SERVER: %s", buf);
           send(sd2,buf,strlen(buf),0);
           closesocket (sd2);
           }
}