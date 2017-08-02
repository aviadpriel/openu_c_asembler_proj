#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define ERROR -1
#define WORD_SIZE 10
typedef enum {ON,OFF,WHIT} SWITCHER;
typedef struct commends
{
  char *command;
}commends;

int isGuidelineStatement(char *p)
{
  int i;
  commends cmd[]={".data",".string",".mat",".entry",".extern"};
  for(i=0;i<5;i++)
  {
    if ((strcmp(cmd[i].command,p))==0)
    return i;
  }
  return -2;
}
char * isLabel(char *p,int *error)
{
  int i;
  while (isspace(*p)) {
  p++;
  }
  if(!(isalpha(*p)&&*p!='.'))
  {
    printf("unligal word : %s \n",p);
    *error=-1;
    return NULL;
  }
  while(p[i]!='\0')
  {
    if(p[i]==':')
    {
      i++;
      if(p[i]=='\0')
      {
        i--;
        p[i]='\0';
        if (!isAction(p)&&!isGuidelineStatement(p)) {
          /* code */
        }
      }
      else
      {
        printf("unligal word : %s \n",p);
        *error=-1;
        return NULL;
      }
    }
  }
  *error=0; /*no error*/
  return NULL;/*but is not a symbol*/
}

char* IsLabel(char *p,int *error)
{
  while (isspace(*p)) {
    p++;
  }
  if(!(isalpha(*p)&&*p!='.'))
  {
    printf("unligal word : %s \n",p);
    *error=ERROR;
    return NULL;
  }
}
