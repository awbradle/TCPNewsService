//Andrew Bradley
//COSC 571
//12-11-16
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void all_news(FILE *news_in);
void search_news(FILE *news_in, char *search_term);

int main()
{
	FILE *news_in;
	news_in = fopen("11-21-2016.txt", "r");
	char news_line[4096];
	fpos_t firstline;
// 	
// 	fgetpos(news_in,&firstline);
// 	fgets(news_line, 4096,news_in);
// 	printf("%s",news_line);
// 	fsetpos(news_in, &firstline);
// 	fgets(news_line, 4096,news_in);
// 	printf("%s",news_line);
	search_news(news_in, "between");
	return 0;
}

void all_news(FILE *news_in)
{
	char news_line[4096];
	int story_counter;
		while(1)
		{
		fgets(news_line, 4096,news_in);
		if(feof(news_in))
		{
			break;
		}
		if(strstr(news_line, "#item"))
		{
			printf("Story:%s",news_line + 5);
		}
		else
		{
			printf("%s",news_line);
		}
	}
	
}

void search_news(FILE *news_in, char *search_term)
{
	char news_line[4096];
	fpos_t firstline;
	fpos_t temp;
	fgetpos(news_in,&firstline);
	
		while(1)
		{
			fgetpos(news_in,&temp);
			fgets(news_line, 4096,news_in);
			if(strstr(news_line, "#item"))
			{
				firstline = temp;
				fgets(news_line, 4096,news_in);
			
			}
			if(strstr(news_line, search_term))
			{
				fsetpos(news_in, &firstline);
				fgets(news_line, 4096,news_in);
				while(strlen(news_line) > 1 && !feof(news_in))
				{
					if(strstr(news_line, "#item"))
					{
						printf("Story:%s",news_line + 5);
					}
					else
					{
						printf("%s",news_line);
					}
					fgets(news_line, 4096,news_in);
				}
			
			}
			if(feof(news_in))
			{
				break;
			}
		}
	
}