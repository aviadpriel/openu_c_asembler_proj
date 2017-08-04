#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ERROR -1
#define LABEL_MAX_LEN 30
typedef enum {ON,OFF,WAIT} SWITCHER;
typedef struct stringArry
    {
    char *string;
    }stringArry;
int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
char * isLabel(char * buf,int *error);
int main()
{
int i;    
char c[]="   aviad:\0";
int error;
char *label=isLabel(c,&error);
printf("error = %d label = %s \n",error,label);    
    return 0;
}


/**/
int isDirective(char *command)
{
    int i=0;
    stringArry cmd[]={
           {".data"},
           {".string"},
           {".mat"},
           {".extern"},
           {".entry"}};
 /*skip spaces and tabs*/   
while(isspace(*command)){command++;}
while(!isspace(command[i])){i++;}
command[i]='\0';
for(i=0;i<5;i++)
{
    if(strcmp(cmd[i].string,command)==0)
    {
     return i;
    }
}
    return -2;
}/*end of isDirective function*/

int isAction(char *command)
{
    int i=0;
    stringArry cmd[]={
           {"mov"},
           {"cmp"},
           {"add"},
           {"sub"},
           {"lea"},
           {"not"},
           {"clr"},
           {"inc"},
           {"dec"},
           {"jmp"},
           {"bne"},
           {"red"},
           {"prn"},
           {"jsr"},
           {"rts"},
           {"stop"}};
            /*skip spaces and tabs*/   
while(isspace(*command)){command++;}
while(!isspace(command[i])){i++;}
command[i]='\0';
for(i=0;i<16;i++)
{
    if(strcmp(cmd[i].string,command)==0)
    {
     return i;
     
    }
}
     return -2;
}/*end of isAction*/


/*This function is designed to check a new label at the beginning of a line*/
char * isLabel(char *buf,int *error)
{
  SWITCHER colonFlag=OFF;/*colon is ":"*/
  int i;
  /*skip spaces and tabs*/
  while (isspace(*buf)) {buf++;}
  /*Check whether the word begins with a letter or "." */
  if((!isalpha(*buf))&&*buf!='.')
  {
    printf("error: unligal word : %s \n",buf);
    *error=ERROR;
    return NULL;
  }
  i=0;
  while(buf[i]!='\0')
  {
    /*Check whether the word contains only letters or numbers*/
    if(!isalnum(buf[i])&&buf[i]!=':')
    {
    printf("error: unligal word : %s \n",buf);
    *error=ERROR;
    return NULL;
    }

    if(buf[i]==':')
    {

    colonFlag=ON;
     /*Check whether the length of the label is less than LABEL_MAX_LEN=30*/   
    if(i>LABEL_MAX_LEN)
    {
        printf("error: Max label length is %d",LABEL_MAX_LEN);
        *error=ERROR;
        return NULL;
    }    
      i++;
      if(buf[i]=='\0')
      {
        i--;
        buf[i]='\0'; /*delete the ":" */
      }
      else
      {
        printf("error: unligal word : %s \n",buf);
        *error=ERROR;
        return NULL;
      }
    }else { i++;}
  }/*end while loop*/
  if(colonFlag!=ON)
  {
    *error=0; /*no error*/
    return NULL;/*but is not a symbol*/
  }

  /*check if the lable is a reserved word*/
    if ((isAction(buf) != -2)||(isDirective(buf) != -2)||isRegister(buf)!=-2) 
    {
      printf("error:  the label cant be a reserved word!!");
      *error= ERROR;
      return NULL;
    }

    /*All tests have been done so word is a labe*/
    *error =0;
    return buf;
}/*end isLabel function */

int isRegister(char *buf)
{
    int i=0;
    
 /*skip spaces and tabs*/   
while(isspace(*buf)){buf++;}
while(buf[i]!='\0' && buf[i]!=' '){i++;}
buf[i]='\0';
if(*buf!='r'|| i!=2)
{
    return -2;
}
buf++;
for(i=0;i<=7;i++)
{
    if((int)*buf-'0'==i)
    {
     return i;
    }
}
     return -2;
}
