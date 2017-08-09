#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ERROR -1
#define LABEL_MAX_LEN 30
int countrChars(char *word,char c,int line);
typedef struct commandList{
char *name;  
int operends;
int firstOperendGroup;
int secondOperendGroup;
}commandList;
typedef enum {EMPTY=-1,IMMEDIATELY=0,DIRECT=1,MATRIX=2,REG_DIRECT=3} addressing_method;
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
char * isLabel(char * buf,int *error,int line);

/**/
int isDirective(char *command)
{
    int functionLen;
    int i=0;
    stringArry cmd[]={
           {".data"},
           {".string"},
           {".mat"},
           {".entry"},
           {".extern"}};
 /*skip spaces and tabs*/   
while(isspace(*command)){command++;}
while(!isspace(command[i])&&command[i]!='\0'&&command[i]!='['){i++;}
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
while(!isspace(command[i])&&command[i]!='\0'){i++;}
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
char * isLabel(char *buf,int *error,int line)
{
  SWITCHER colonFlag=OFF;/*colon is ":"*/
  int i;
  /*skip spaces and tabs*/
  while (isspace(*buf)) {buf++;}
  /*Check whether the word begins with a letter or "." */
  if((!isalpha(*buf))&&*buf!='.')
  {
    printf("error:line %d: unligal word : %s \n",line,buf);
    *error=ERROR;
    return NULL;
  }
  i=0;
  while(buf[i]!='\0')
  {
    /*Check whether the word contains only letters or numbers*/
    if(!isalnum(buf[i])&&buf[i]!=':'&&buf[0]!='.')
    {
    printf("error:line %d: unligal word : %s \n",line,buf);
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
    printf("error:line %d: unligal word : %s \n",line,buf);
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
      printf("error:line %d:  the label cant be a reserved word!!",line);
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
  int labelLen;
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
  labelLen=i;
  /*check if the lable is a reserved word*/
    if ((isAction(buff) != -2)||(isDirective(buff) != -2)||isRegister(buff)!=-2) 
    {
        printf("error:line %d: operend cannot be a reserved word!! \n",line);
      return ERROR;
    }
    i++;
    while (isspace(buff[i])) {i++;}/**/
    if(buff[i]!='\0')
    return ERROR;
    /*All tests have been done so word is a labe
    test the len of the label
    */
    if(labelLen>LABEL_MAX_LEN)
    {
        printf("ERROR:line %d:max lenght of label is %d ",LABEL_MAX_LEN,line);
        return ERROR;
    }
    /*All tests have been done so word is a labe*/
    return 1;
}/*end isDirectiveAddressing function */

/*isRegister:
if is a register it return the index of the register (0-7)
else it return -2 
*/
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
/*   */
int isMatrix(char *buff,int line,int * r1,int *r2)
{
char *label =NULL,*temp =NULL;    
char openBracket='[',closeBracket=']';
int openCunter=0,closeCunter=0,i=0,rval;
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
            return 0;
        } 
    }
    i++;
}
if(closeCunter!=2 ||openCunter !=2)
{
 return 0;
}
/*check the label*/
 while (isspace(*buff)) {buff++;}
 i=0;
 while(buff[i]!=openBracket ||isspace(buff[i])){i++;}
 label=(char *)malloc(sizeof(char)*i);
 if(!label)
 {
     printf("mermory error !! exit");
     exit(1);
 }
 strncpy(label,buff,i);
 if(isDirectiveAddressing(label,line)!=1)
 {
     return 0;/*the lable is un ligal*/
 }
 /*if evgeny need this function TODO;return the label*/
 free(label);
 buff+=i;
while(isspace(*buff)){buff++;}
if(*buff!=openBracket)
{
    return 0;
}
buff++;
i=0;
while(buff[i]!=closeBracket){i++;}
temp=(char*)malloc(sizeof(char)*i);
strncpy(temp,buff,i);
rval=isRegister(temp);
if(rval==-2)
{
free(temp);    
return 0;
}
if(r1)
{
*r1=rval;
}
buff+=i+1;

while(isspace(*buff)){buff++;}
if(*buff!=openBracket)
{
    return 0;
}
buff++;
memset(temp,'\0',i);
i=0;
while(buff[i]!=closeBracket){i++;}
strncpy(temp,buff,i);
rval=isRegister(temp);
if(rval==-2)
{
return 0;
}
if(r2)
{
*r2=rval;
}
buff+=i;

return 1;
}

int identification(char * buff,int functionIndex,int line,int *ic)
{
addressing_method firstOp,secondOp;    
int firstCost=0,secondCost=0,rval=0;    
commandList cmd[]={{"mov",2,4,3},
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

if(cmd[functionIndex].operends==0)/*if no operends*/
{
    if(buff==NULL)
    {
        *ic+=1;/*for PSW*/
        return 0;
    }

 while(!isspace(*buff)){buff++;}
 if(*buff!='\0')
 {
   printf("no need operends for %s function.\n",cmd[functionIndex].name);  
   return ERROR;  
 }
    *ic+=1;/*for PSW*/
    return 0;
}else if(cmd[functionIndex].operends==1)
{
    if(strcmp("prn",cmd[functionIndex].name)==0)
    {
    rval=isImmediateAddressing(buff,line);
    if(rval==1)
    {
     *ic+=2;   
    }else if(rval == ERROR)
    {
        return ERROR;
    }
    return 0;
    }
    rval=isDirectiveAddressing(buff,line);
    if(rval==1)
    {
     *ic+=2;
      return 0;   
    }else if(rval == ERROR)
    {
        return ERROR;
    }
    rval=isMatrix(buff,line,NULL,NULL);/*NULL becuse we donte need the regestres*/
    if(rval==1)
    {
     *ic+=3; /*1 for PSW and 2 for the metrix*/
      return 0;   
    }else if(rval == ERROR)
    {
        return ERROR;
    }
    rval=isRegister(buff);
    if(rval!=-2)
    {
     *ic+=2;/*1 for PSW and 1 for the register*/
      return 0;   
    }else if(rval == ERROR)
    {
        return ERROR;
    } 
    
    printf("ERROR:line %d:unvalid dest opernd",line);
    return ERROR;
} else if(cmd[functionIndex].operends==2)
    {
        int i=countrChars(buff,',',line);
        char *tok;
        if(i!=1)
        {
            printf ("ERROR:line %d:no , between the operends",line);
            return ERROR;
        }
        
        firstOp=EMPTY;
        secondOp=EMPTY;
        /*calculet the first operend*/
        tok=strtok(buff,",");
        rval=isImmediateAddressing(tok,line);
        if(rval == ERROR)
        {
            return ERROR;
        }else if(rval==1)
        {
            firstOp=IMMEDIATELY;
        }
        if(firstOp==EMPTY)
        {
            rval=isMatrix(tok,line,NULL,NULL);/*NULL becuse we donte need the regestres*/
            if(rval == ERROR){return ERROR;}
            else if(rval==1){firstOp=MATRIX;}
        }
       if(firstOp==EMPTY)
        {
            rval=isRegister(tok);
            if(rval!=-2){firstOp=REG_DIRECT;}
        }
        if(firstOp==EMPTY)
        {
            rval=isDirectiveAddressing(tok,line);
            if(rval == ERROR){return ERROR;}
            else if(rval==1){firstOp=DIRECT;}
        }
        if(firstOp==EMPTY)
        {
            printf("ERROR:line %d: unvalid addressing method ",line);
            return ERROR;
        }
        /*calculet the second operend*/
         tok=strtok(NULL,"\0");
        rval=isImmediateAddressing(tok,line);
        if(rval == ERROR)
        {
            return ERROR;
        }else if(rval==1)
        {
            secondOp=IMMEDIATELY;
        }
        if(secondOp==EMPTY)
        {
            rval=isMatrix(tok,line,NULL,NULL);/*NULL becuse we donte need the regestres*/
            if(rval == ERROR){return ERROR;}
            else if(rval==1){secondOp=MATRIX;}
        }
       if(secondOp==EMPTY)
        {
            rval=isRegister(tok);
            if(rval!=-2){secondOp=REG_DIRECT;}
        }
        if(secondOp==EMPTY)
        {
            rval=isDirectiveAddressing(tok,line);
            if(rval == ERROR){return ERROR;}
            else if(rval==1){secondOp=DIRECT;}
        }
        if(secondOp==EMPTY)
        {
            printf("ERROR:line %d: unvalid addressing method ",line);
            return ERROR;
        }
    
    /*test if the  oprends is a ligals oprends*/
    if(functionIndex==4)/*4 == lea in the cmd[]arry*/
    {
        if(firstOp!=DIRECT&&firstOp!=MATRIX)
        {
        printf("ERROR:line %d:lea can get in the first operend only DIRECT OR MATRIX METHOD !!",line);
        return ERROR;
        }
        if(secondOp==IMMEDIATELY)
        {
        printf("ERROR:line %d:lea can't get in the second operend a IMMEDIATELY METHOD !!",line);
        return ERROR;
        }
    }

    if(secondOp==IMMEDIATELY&&functionIndex!=12&&functionIndex!=1)/*the function is not cmp or prn*/
    {
        printf("ERROR:line %d:only cmp or prn can get in the second operend in a IMMEDIATELY METHOD !!",line);
        return ERROR;
    }

    /*if we got here the oprends is ligal*/
    switch(firstOp)
    {
        case IMMEDIATELY:
        case DIRECT:
        case REG_DIRECT:
        {
        firstCost =1;
        break;
        }
        case MATRIX:
        {
          firstCost=2;
          break;  
        }
          case EMPTY:
        {
        return ERROR;
        }
    }
    switch(secondOp)
    {
        case IMMEDIATELY:
        case DIRECT:
        case REG_DIRECT:
        {
        secondCost =1;
        break;
        }
        case MATRIX:
        {
          secondCost=2;
          break;  
        }
        case EMPTY:
        {
        return ERROR;
        }
    }
    if(firstOp==REG_DIRECT&&secondOp==REG_DIRECT)
    {
        *ic+=2;/*1 for psw and 1 fo the common word for the registers*/
        return 0;
    }

    *ic+=(secondCost+firstCost + 1);/*1 for psw*/
    return 0;

    }
    return ERROR;
}
 
