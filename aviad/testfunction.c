#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ERROR -1
typedef enum {ON,OFF,WHIT} SWITCHER;
char * isSymbol(char *p,int *error);
int isGuidelineStatement(char *p);
typedef struct commends
{
char *command;
}commends;
/* HELLO: add r1,2
          sub r2,2
          .mat [2][2] 1,2,3,4
          .string "aviad"

*/

/*
pasado code
is label ? flag_on:flag_off;
is guidelineStatement ?
guidelineStatement(line,flag_on,label) or(line,flag_off,NULL):
next

*/
int testline(char *line,int dc,int ic)
{
SWITCHER lableFlag=OFF;
int errorFlag=0,intdex =-1;
char *tok,*label;
tok=strtok(line," ");
label=isSymbol(tok,&errorFlag);
if (errorFlag==ERROR)
{
  return ERROR;
}
else if (errorFlag=1)/*is a leable*/
{
lableFlag = ON;
tok=strtok(NULL," ");
}
if ((intdex=isGuidelineStatement(tok))!=-2) {

}


}

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


void guidelineStatement(char *commend,int commandIndex,int dc)
{
  char *temp =commend;
  switch (commandIndex) {
    case 0:
    {

    }
    case 1:
    {

    }
    case 2:
    {

    }
  }
}

char * isSymbol(char *p,int *error)
{
  int i=0;
  if(!(isalpha(p[i])&&p[i]!='.'))
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
          *error=1;
          return p;/*is symbol!!!*/
        }
          else
          {
          printf("unligal word : %s \n",p);
          *error=-1;
          return NULL;
          }
      }
      else
      i++;
  }
*error=1; /*no error*/
return NULL;/*but is not a symbol*/
}
/* .data 5,2,6,7*/
int dataF(char *command,char * label,SWITCHER lableFlag)
{
  while (/* condition */) {
    /* code */
  }
  return 0;
}
