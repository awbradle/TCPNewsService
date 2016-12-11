#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>
#include <string.h>

#define RCVBUFSIZE 4096   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    FILE *news_in;
	news_in = fopen("11-21-2016.txt", "r");
	fpos_t firstline;

    /* Receive message from client */
   recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
   if(strstr(echoBuffer, "#allnews"))
   {
       /* Send received string and receive again until end of transmission */
		while (1)
		{
			fgets(echoBuffer, 4096,news_in);
			if(feof(news_in)|| strlen(echoBuffer)< 1)
			{
				break;
			}
			/* Echo message back to client */
			if(strstr(echoBuffer, "#item"))
			{
				echoBuffer[0] = 'i';
				echoBuffer[1] = 't';
				echoBuffer[2] = 'e';
				echoBuffer[3] = 'm';
				echoBuffer[4] = ':';
				send(clntSocket, echoBuffer, strlen(echoBuffer), 0);
			}
			else
			{
				send(clntSocket, echoBuffer, strlen(echoBuffer), 0);
			}
		}
   }

	fclose(news_in);
    close(clntSocket);    /* Close client socket */
}
