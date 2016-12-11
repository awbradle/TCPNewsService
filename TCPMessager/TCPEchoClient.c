#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 4096   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[])
{
    int user_choice;
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv()
                                        and total bytes read */
    
    if ((argc != 3))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> <Server Port>\n",
               argv[0]);
       exit(1);
    }
    
    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Use given port, if any */
    
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */
	
	while(user_choice != 3)
		{
		printf("1. Get all news\n");
		printf("2. Search for a keyword in today's news\n");
		printf("3. Quit\n");
		printf("Please enter your choice: ");
		scanf("%d",&user_choice);
		
		if(user_choice == 1)
		{
			strcpy(echoBuffer, "#allnews");
		}
		else if(user_choice == 2)
		{
			printf("Enter the term you wish to search for in today's news: ");
			scanf("%s", echoBuffer);
		}
		else
		{
			exit(0);
		}
	servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]); /* Use given port, if any */
    
		/* Create a reliable, stream socket using TCP */
		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			DieWithError("socket() failed");
	
		/* Construct the server address structure */
		memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
		echoServAddr.sin_family      = AF_INET;             /* Internet address family */
		echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
		echoServAddr.sin_port        = htons(echoServPort); /* Server port */
		
		/* Establish the connection to the echo server */
		if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
			DieWithError("connect() failed");
	
		echoStringLen = strlen(echoBuffer);          /* Determine input length */
	
		/* Send the string to the server */
		if (send(sock, echoBuffer, echoStringLen, 0) != echoStringLen)
			DieWithError("send() sent a different number of bytes than expected");
	
		printf("Retrieving your news:\n");                /* Setup to print the echoed string */
		while (1)
		{
			/* Receive up to the buffer size (minus 1 to leave space for
			   a null terminator) bytes from the sender */
			if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) < 0)
				DieWithError("recv() failed");
			if(!bytesRcvd)
        		break;
			totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
			echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
			printf("%s", echoBuffer);      /* Print the echo buffer */
		}
		printf("\n");    /* Print a final linefeed */
		close(sock);
    }
    exit(0);
}
