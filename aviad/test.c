#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define ERROR -1
#define LINE_LEN 81
#define BUFF_LEN 100
typedef enum {ON,OFF,WHIT} SWITCHER;
int main()
{
FILE *fp;
char * buff;
buff=(char *)malloc(sizeof(char)*BUFF_LEN);
fp=fopen("test.t","r");
if(!fp)
{
  exit(1);
}
while(fgets( buff, BUFF_LEN,fp))
{
  if(strlen(buff)>=LINE_LEN)
  {
    printf("aviad test aaaaaaaaaaaa");
  }
}
return 0;
}