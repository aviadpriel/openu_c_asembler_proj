#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int isDirectiveAddressing(char * buff,int line);/*from isSome.c*/
#define ERROR -1
int countrChars(char *word, char c,int line);
/*get numbers from comma list like 2,3,6,4,8,9*/
int commaList(dataList **dataHead, char *command, int cummaCounter, int *dc,int line);
typedef struct commends
{
  char *command;
} commends;
/* .data 5,2,6,7*/
int dataF(char *command, char *label,int *dc, dataList **dataHead, labelsList **labelsHead,int line)
{
  int cummaCounter = 0, i = 0, currDc;
  currDc = *dc;
  /*count commas to get how much numbers we have to insern*/
  while (command[i] != '\0')
  {
    if (command[i] == ',')
    {
      cummaCounter++;
      if (command[i + 1] == ',')
      {
        printf("error:line %d; double commas \n",line);
        return ERROR;
      }
    }
    i++;
  }

  if ((commaList(dataHead, command, cummaCounter, dc,line)) == ERROR)
    return ERROR;
  if (label)
  {
    int num = addLabel(labelsHead,label,OFF,OFF,ON,currDc,line);
    if (!num)
    {
      return ERROR;
    }
    else
    {
      return 0;
    }
  }
  return 0;
}
/*
  HELLO: .string "aviad"
  "aviad"
  */
int stringF(char *command, char *label, int *dc, dataList **dataHead, labelsList **labelsHead,int line)
{
  int currDc = *dc, i;
  int countr = countrChars(command, '\"',line);
  int data;
  if (countr != 2)
  {
    return ERROR;
  }
  i = 0;
  while (command[i] != '\"')
  {
    if (!isspace(command[i]))
    {
      printf("error:line %d: in format of .string \"somestring\"  %s\n",line,command);
      return ERROR;
    }
    i++;
  }
  i++; /*for pass the " char*/
  while ((data = command[i]) != '\"')
  {
    insertData(dataHead, data);
    (*dc)++;
    i++;
  }
  /*add a 0 in the and if the string*/
  insertData(dataHead, 0);
  (*dc)++;
  /*Check with the rest of the line without characters*/
  i++;
  while (command[i] != '\0')
  {
    if (!isspace(command[i]))
    {
      printf("error:line %d: in format of .string \"somestring\"  %s\n",line,command);
      return ERROR;
    }
    i++;
  }
  /*and Check... */

  /*add label if exzist one */
  if (label)
  {
    int num = addLabel(labelsHead, label, OFF, OFF,ON,currDc,line);
    if (!num)
    {
      return ERROR;
    }
    else
    {
      return 0;
    }
  }
  return 0;
}

int matF(char *buff,char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line)
{

  int currDc = *dc, counter, matRow, matColum, matLen,diff;
  char openBracket='[',closeBracket=']';
int openCunter=0,closeCunter=0,i=0;
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
 return ERROR;
}

  while (*buff != '[')
  {
    if (!isspace(*buff))
    {
      printf("error:line %d: in .mat [num1][num2] format: .mat n",line);
      return ERROR;
    }
    buff++;
  }
  /*We will examine the first casings*/
  buff++;
  matRow = (int)strtol(buff, &buff, 10);
  while (*buff != ']')
  {
    if (!isspace(*buff))
    {
      printf("error:line %d: in .mat [num1][num2] format: .mat n",line);
      return ERROR;
    }
    buff++;
  }
  buff++;
   while (*buff != '['&&buff!=NULL)
  {
    if (!isspace(*buff))
    {
      printf("error:line %d: in .mat [num1][num2] format: .mat n",line);
      return ERROR;
    }
    buff++;
  }
  if (*buff != '[')
  {
      printf("error:line %d: in .mat [num1][num2] format: .mat n",line);
    return ERROR;
  }
    buff++;
  matColum = (int)strtol(buff, &buff, 10);
  while (*buff != ']')
  {
    if (!isspace(*buff))
    {
      printf("error:line %d: in .mat [num1][num2] format: .mat n",line);
      return ERROR;
    }
    buff++;
  }
   buff++;
  counter = countrChars(buff, ',',line);
  if (counter == ERROR)
    return ERROR;

  matLen = matColum * matRow;
  if (!matLen)
  {
    printf("error:line %d: in .mat [num1][num2] format: .mat n",line);
    printf("num1 and num2 Must be greater than 0 n");
    return ERROR;
  }
  
  /*if No entries have been entered, we will save a space in datalist*/
/* TODO add test for chack if we have one variable*/
  if (counter == 0)
  {
    
    for (; matLen > 0; matLen--)
    {
      insertData(dataHead, 0);
      (*dc)++;
    }
  }
  else if(counter>=matLen)
  {
  printf(" error:line %d too many arguments to function .mat \n",line);
  return ERROR;
  }
  else
  {
    if (commaList(dataHead, buff, counter, dc,line) == ERROR)
      return ERROR;
  }
    if(matLen>(counter+1))
    {
    diff = matLen-(counter+1);
    for (; diff > 0; diff--)
    {
      insertData(dataHead, 0);
      (*dc)++;
    }
    }  

  /*add label if exzist one */
  if (label)
  {
    int num = addLabel(labelsHead, label, OFF, OFF,ON,currDc,line);
    if (!num)
    {
      return ERROR;
    }
    else
    {
      return 0;
    }
  }
  return 0;
} /*end of matF function*/
int externF(char *buff,char *label,labelsList **labelsHead,int line)
{
  int cunter=countrChars(buff,',',line);
   if(label)
          {
            printf("warnning:line %d:no need lable for .extern function.\n",line);
          }
  if(cunter == ERROR)
    {
      return ERROR;
    } 

    if(cunter == 0)
    {
      if(isDirectiveAddressing(buff,line)==1)
      {
        addLabel(labelsHead,buff,WAIT,ON,WAIT,0,line);
      }else
        {
          printf("error:line %d: unvalid label for extern function !!\n",line);
          return ERROR;
        }
    }else
    {
      char *tok;
      tok=strtok(buff,",");
      for(;cunter>=0;cunter--)
    {
      if(!tok)
      {
        printf("error:line %d: un use comma in the end of the line\n",line);
        return ERROR;
      }
      else 
      {
        int i=0;
        while(isspace(tok[i])&&tok[i]!='\0'){i++;}
        if(tok[i]=='\0')
        {
          printf("error:line %d: no data between the commas \n ",line);
          return ERROR;
        }
      }
       if(isDirectiveAddressing(tok,line)!=1)
      {
        addLabel(labelsHead,tok,WAIT,ON,WAIT,0,line);
      }else
        {
          printf("error:line %d: unvalid label for extern function !!\n",line);
          return ERROR;
        }
     tok=strtok(NULL,","); 
    }
    return 0;
    }
        return 0;
}
int entryF(char * buff,char *label,int line)
{
  int rval;
    if(label)
          {
            printf("warnning:line %d:no need lable for .entry function.\n",line);
          }
 if((rval=isDirectiveAddressing(buff,line))==ERROR)
 {
  return ERROR;
 }
 else if(rval ==1)
 {
 return 1;
 }
 /*rval = 0*/
 printf("error : line %d: unligal label!!\n",line);
 return ERROR;
}
 int countrChars(char *word, char c,int line)
{
  int counter = 0;
  int i=0;
  while (word[i] != '\0')
  {
    if (word[i] == c)
    {
      i++;
      counter++;
      if (word[i] == c)
      {
        printf("error:line %d: double %c is unligal!!\n",line,c);
        return ERROR;
      }
    }
    i++;
  } /*end of while*/
  return counter;
}
int commaList(dataList **dataHead, char *command, int cummaCounter, int *dc,int line)
{
  char *cp;
  int data;
  /*get the data to the Datalist*/
  cp = strtok(command, ",");
  if (cummaCounter == 0)
  { /*we have only one number*/
    data = (int)strtol(cp, &cp, 10);
    while (*cp == ' ')
    {
      cp++;
    }
    if (*cp != '\0')
    {
      printf("error:line %d: un prespectiv varible!! %c \n",line,*cp);
      return ERROR;
    }else{
        insertData(dataHead, data);
      *dc+=1;
      return 0;
    }
    
  }
  for (; cummaCounter >= 0; cummaCounter--)
  {
    if(!cp)
    {
      printf("i found the bug\n");
      exit(1);
    }else 
    {
      int i=0;
      while(isspace(cp[i])&&cp[i]!='\0'){i++;}
      if(cp[i]=='\0')
      {
        printf("error:line %d: no data between two commas \n ",line);
        return ERROR;
      }

    }
    data = (int)strtol(cp, &cp, 10);
    /*test if cp is empty*/
    while (*cp == ' ')
    {
      cp++;
    }
    if (*cp != '\0')
    {
      printf("error:line %d: un prespectiv varible!! %c \n",line,*cp);
      return ERROR;
    }
    else
    {
      insertData(dataHead, data);
      *dc+=1;
    }
    cp = strtok(NULL, ",");
  }
  return 0;
}
