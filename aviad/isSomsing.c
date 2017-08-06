#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ERROR -1
#define LABEL_MAX_LEN 30
typedef enum addressing_method{IMMEDIATELY, DIRECT, INDEX, REG_DIRECT} addressing_method;
/*whte we have in this file
isDirective -work
isAction -work
isRegister -work
isLabel - need more tests
*/
typedef enum {ON,OFF,WAIT} SWITCHER;
typedef struct stringArry
    {
    char *string;
    }stringArry;
int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
char * isLabel(char * buf,int *error);

/**/
int isDirective(char *command)
{
    int functionLen;
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
functionLen = i;
for(i=0;i<5;i++)
{
    if(strncmp(cmd[i].string,command,functionLen)==0)
    {
     return i;
    }
}
    return -2;
}/*end of isDirective function*/

int isAction(char *command)
{
    int i=0,functionLen=0;
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
functionLen = i;
for(i=0;i<16;i++)
{
    if(strncmp(cmd[i].string,command,functionLen)==0)
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


/*6/8/17 work on ic calculate*/

/*functions for Addressing methods*/

int isImmediateAddressing(char * buff,int line)
{
    char *rest;
    char asterisk='#';
    while(isspace(*buff)){buff++;}
    if(*buff!=asterisk){return 0;}/*is not a immediate addressing */
    buff++;/*skip the asterisk */
    if ((!isdigit(*buff))&&(*buff!='-')&&(*buff!='+'))
    {
    printf("ERROR:line %d: unligal operend!!\n",line);
    return ERROR;
    }
    strtol(buff,&rest,10);
    while(isspace(*rest)) rest++;
    if(*rest!='\0')return ERROR;/*Because an asterisk has only an immediate address*/ 
    return 1;/*buff is Immediate Addressing operend */
}

int isDirectiveAddressing(char * buff,int line)
{
  int i;
  /*skip spaces and tabs*/
  while (isspace(*buff)) {buff++;}

  if((!isalpha(*buff)))
  {
    return 0;/*is not a label*/
  }
  i=0;
  while(!isspace(buff[i])&&buff[i]!='\0')
  {
    /*Check whether the word contains only letters or numbers*/
    if(!isalnum(buff[i])){return 0;}   
   i++;
  }/*end while loop*/
  /*check if the lable is a reserved word*/
    if ((isAction(buff) != -2)||(isDirective(buff) != -2)||isRegister(buff)!=-2) 
    {
      return ERROR;
    }
    i++;
    while (isspace(buff[i])) {i++;}/**/
    if(buff[i]!='\0')
    return ERROR;

    /*All tests have been done so word is a labe*/
    return 1;
}/*end isDirectiveAddressing function */

int isRegister(char *buf)
{
    int i=0;
    
 /*skip spaces and tabs*/   
while(isspace(*buf)){buf++;}
while(buf[i]!='\0' && buf[i]!=' '){i++;}
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
/* a[r1][r2]  *//*
int isMatrix(char *buff,int line)
{
char openBracket='[',closeBracket=']';
int openCunter=0,closeCunter=0,i;
while(buff[i]!='\0')
{
    if(buff[i]==openBracket)
    {
        openCunter++;
        if(openCunter==2 &&closeCunter=0)
        {
            return ERROR;
        } 
    }else if(buff[i]==closeCunter)
    {
        closeCunter++;
        if(closeCunter>openCunter=0)
        {
            return ERROR;
        } 
    }
    i++;
}
if(closeCunter!=2 ||openCunter !=2)
{
 return ERROR;
}

while (isspace(*buff)) {buff++;}
if(*buff!=openBracket)
    return ERROR;
buff++;/*SKIP THE openBracket

 while (isspace(*buff)) {buff++;}
*/   

