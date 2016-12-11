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
    char search_term[RCVBUFSIZE];
    FILE *news_in;
	news_in = fopen("11-21-2016.txt", "r");
	fpos_t firstline;
	fpos_t temp;
	fgetpos(news_in,&firstline);
	int recv_size;

    /* Receive message from client */
   recv_size = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0);
   echoBuffer[recv_size] = '\0';
   strcpy(search_term,echoBuffer);
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
   else
   {
   		while(1)
   		{
   		fgetpos(news_in,&temp);
			fgets(echoBuffer, 4096,news_in);
			if(strstr(echoBuffer, "#item"))
			{
				firstline = temp;
				fgets(echoBuffer, 4096,news_in);
			}
			if(strstr(echoBuffer, search_term))
			{
				fsetpos(news_in, &firstline);
				fgets(echoBuffer, 4096,news_in);
				while(strlen(echoBuffer) > 1 && !feof(news_in))
				{
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
					fgets(echoBuffer, 4096,news_in);
				}
			
			}
   			if(feof(news_in))
			{
				break;
			}
   		}
   }

	fclose(news_in);
    close(clntSocket);    /* Close client socket */
}
