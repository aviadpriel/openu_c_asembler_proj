#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "consts.h"
/*פונקציות עזר*/
 int isEmpty(char * buff);
int countrChars(char *word,char c,int line);/*from testFunction.c*/
/*מבנה נתונים לאחסון הפונקציות*/
typedef struct commandList{
char *name;  
int operends;
int firstOperendGroup;
int secondOperendGroup;
}commandList;
/*לזיהוי סוג המיעון*/
static commandList _action[]={{"mov",2,4,3},
				{"cmp",2,4,4},
				{"add",2,4,3},
				{"sub",2,4,3},
				{"lea",2,2,3},
				{"not",1,0,3},
				{"clr",1,0,3},
				{"inc",1,0,3},
				{"dec",1,0,3},
				{"jmp",1,0,3},
				{"bne",1,0,3},
				{"red",1,0,3},
				{"prn",1,0,4},
				{"jsr",1,0,3},
				{"rts",0,0,0},
				{"stop",0,0,0},
				{NULL,0,0,0}};
static commandList _directive[]={
           {".data"},
           {".string"},
           {".mat"},
           {".entry"},
           {".extern"}};

int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
char * isLabel(char * buf,int *error,int line);

/*chack if the line is a comment
returns
if is comment TRUE
else NOT_EXIST 
*/
int isComment(char * buff)
{
    int i=0;
    while(isspace(buff[i])){i++;}
    if(buff[i]==COMMENT_FLAG)
    {
        return TRUE;
    }
    return NOT_EXIST;
}
/*get a word and chack if is a name of
* directive function
*if is a name of directive function
*return the index of the function (0-4)
*else return NOT_EXIST
*/
int isDirective(char *command)
{
    int functionLen;
    int i=0;
 /*skip spaces and tabs*/   
while(isspace(*command)){command++;}
/*for the case .mat[][] */
while(!isspace(command[i])&&command[i]!='\0'&&command[i]!='['){i++;}
functionLen = i;
for(i=0;i<5;i++)
{
    if(strncmp(_directive[i].name,command,functionLen)==TRUE)
    {
        if((strlen(_directive[i].name))==(functionLen)) 
            return i;
        else 
            {

            }
    }
}
    return NOT_EXIST;
}/*end of isDirective function*/

/*get a word and chack if is a name of
* action function
*if is a name of action function
*return the index of the function (0-15)
*else return NOT_EXIST
*/
int isAction(char *command)
{
    int i=0,functionLen=0;
    /*skip spaces and tabs*/   
    while(isspace(*command)){command++;}
    while(!isspace(command[i])&&command[i]!='\0'){i++;}
    functionLen = i;
    for(i=0;i<16;i++)
    {
        if(strncmp(_action[i].name,command,functionLen)==TRUE)
        {
            if((strlen(_action[i].name))==(functionLen)) 
                return i;
        }
    }
    return NOT_EXIST;
}/*end of isAction*/


/*This function is designed to check a new label at the beginning of a line
*it return a the label in char *
*else it return NULL (if is not label or have some error)
* also if have some error it put in (int *error) ERROR 
*/
char * isLabel(char *buf,int *error,int line)
{
  SWITCHER colonFlag=OFF;/*colon is ":"*/
  int i;
  /*skip spaces and tabs*/
  while (isspace(*buf)) {buf++;}
  /*Check whether the word begins with a letter or "." */
  if((!isalpha(*buf))&&*buf!='.')
  {
    printf("error:line %d: Invalid word / phrase : %s \n",line,buf);
    *error=ERROR;
    return NULL;
  }
  /*if buf can be a directiv function*/
  if(*buf=='.')
    {
    *error=TRUE; /*no error*/
    return NULL;/*but is not a label*/
    }
  i=0;
  while(buf[i]!='\0')
  {
    /*Check whether the word contains only letters or numbers*/
    if(!isalnum(buf[i])&&buf[i]!=':'&&buf[0]!='.'&&buf[i]!='\n')
    {
        printf("error:line %d: Invalid word / phrase : %s \n",line,buf);
        *error=ERROR;
        return NULL;
    }

    if(buf[i]==':')
    {

    colonFlag=ON;
     /*Check whether the length of the label is less than LABEL_MAX_LEN=30*/   
    if(i>LABEL_MAX_LEN)
    {
        printf("error:line %d: Max label length is %d",line,LABEL_MAX_LEN);
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
    printf("error:line %d: Invalid word / phrase : %s \n",line,buf);
        *error=ERROR;
        return NULL;
      }
    }else { i++;}
  }/*end while loop*/
  if(colonFlag!=ON)
  {
    *error=TRUE; /*no error*/
    return NULL;/*but is not a label*/
  }

  /*check if the lable is a reserved word*/
    if ((isAction(buf) != NOT_EXIST)||(isDirective(buf) != NOT_EXIST)||isRegister(buf)!=NOT_EXIST) 
    {
      printf("error:line %d:  the label cant be a reserved word!!",line);
      *error= ERROR;
      return NULL;
    }

    /*All tests have been done so word is a labe*/
    *error =TRUE;
    return buf;
}/*end isLabel function */

/************************************************/
/*functions for Addressing methods

get a operend of action function
end
return ERROR if have some error
else
return VALID if is the Addressing method
else
return NOT_EXIST if is not the Addressing method
*/

/*immediate addressing*/
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
    return VALID;/*buff is Immediate Addressing operend */
}/*end of isImmediateAddressing*/

/*directive addressing*/
int isDirectiveAddressing(char * buff,int line)
{
  int i;
  int labelLen;
  /*skip spaces and tabs*/
  while (isspace(*buff)) {buff++;}

  if((!isalpha(*buff)))
  {
    return NOT_EXIST;/*is not a label*/
  }
  i=0;
  while(!isspace(buff[i])&&buff[i]!='\0')
  {
    /*Check whether the word contains only letters or numbers*/
    if(!isalnum(buff[i])){return 0;}   
   i++;
  }/*end while loop*/
  labelLen=i;
  /*check if the lable is a reserved word*/
  if ((isAction(buff) != NOT_EXIST)||(isDirective(buff) != NOT_EXIST)||isRegister(buff)!=NOT_EXIST) 
  {
    printf("error:line %d: operend cannot be a reserved word!! \n",line);
    return ERROR;
  }
    
    while (isspace(buff[i])&&buff[i]!='\n') {i++;}/**/
    if(buff[i]!='\0')
    {
    printf("error !!! = %c  \n",buff[i]) ;   
    return ERROR;
    }
    /*All tests have been done so word is a labe
    test the len of the label
    */
    if(labelLen>LABEL_MAX_LEN)
    {
        printf("ERROR:line %d:max lenght of label is %d ",LABEL_MAX_LEN,line);
        return ERROR;
    }
    /*All tests have been done so word is a labe*/
    return VALID;
}/*end isDirectiveAddressing function */

/*isRegister:
if is a register it return the index of the register (0-7)
else it return NOT_EXIST 
*/
int isRegister(char *buf)
{
    int i=0;
    
 /*skip spaces and tabs*/   
while(isspace(*buf)){buf++;}
while(buf[i]!='\0' && buf[i]!=' '){i++;}
if(*buf!='r'|| i!=2)
{
    return NOT_EXIST;
}
buf++;
for(i=0;i<=7;i++)
{
    if((int)*buf-'0'==i)
    {
     return i;
    }
}
     return NOT_EXIST;
}
/*  Matrix addressing  
* if r1 and r2 not equal to null
* it put the row size in r1 and the
*colume size in r2 
*/
int isMatrix(char *buff,int line,int * r1,int *r2)
{
    char *label,*temp =NULL;    
    char openBracket='[',closeBracket=']';
    int openCunter=0,closeCunter=0,i=0,rval;
    label=NULL;
    while(buff[i]!='\0')
    {
        if(buff[i]==openBracket)
        {
            openCunter++;
            if(openCunter==2 &&closeCunter==0)
            {
                return ERROR;
            } 
        }else if(buff[i]==closeBracket)
        {
            closeCunter++;
            if(closeCunter>openCunter)
            {
                return NOT_EXIST;
            } 
        }
        i++;
    }
    if(closeCunter!=2 ||openCunter !=2)
    {
        return NOT_EXIST;
    }
/*check the label*/
    while (isspace(*buff)) {buff++;}
    i=0;
    while(buff[i]!=openBracket&&!isspace(buff[i])){i++;}
    label=(char *)malloc(sizeof(char)*(i+1));
    if(!label)
    {
        printf("mermory error !! exit");
        exit(1);
    }
    memset(label,'\0',i+1);
    strncpy(label,buff,i);
    if(isDirectiveAddressing(label,line)!=VALID)
    {
        return NOT_EXIST;/*the lable is un ligal*/
    }
    free(label);
    buff+=i;
    while(isspace(*buff)){buff++;}
    if(*buff!=openBracket)
    {
        return NOT_EXIST;
    }
    buff++;
    i=0;
    while(buff[i]!=closeBracket){i++;}
    temp=(char*)malloc(sizeof(char)*i);
    strncpy(temp,buff,i);
    rval=isRegister(temp);
    if(rval==NOT_EXIST)
    {
        free(temp);    
        return NOT_EXIST;
    }
    if(r1)
    {
    *r1=rval;
    }
    buff+=i+1;

    while(isspace(*buff)){buff++;}
    if(*buff!=openBracket)
    {
        return NOT_EXIST;
    }
    buff++;
    memset(temp,'\0',i);
    i=0;
    while(buff[i]!=closeBracket){i++;}
    strncpy(temp,buff,i);
    rval=isRegister(temp);
    if(rval==NOT_EXIST)
    {

        return NOT_EXIST;
    }
    if(r2)
    {
        *r2=rval;
    }
    buff+=i;

    return VALID;
}/*end of isMatrix*/

/*end of functions for addressing methods */
/***************************************************/
 int isEmpty(char * buff)
 {
    int i=0;
    if(!buff)
    {
        return 0;
    }
     while(buff&&isspace(buff[i])){i++;}
     if((!buff[i]))
     {
         return 0;
     }
     return NOT_EXIST;
 }/*end of identification*/

 /*end of file*/
