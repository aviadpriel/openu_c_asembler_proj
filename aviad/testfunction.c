#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "consts.h"
int isDirectiveAddressing(char * buff,int line);/*from isSome.c*/
int countrChars(char *word, char c,int line);
int isEmpty(char * buff);/*from isSomsing.c*/
int commaList(dataList **dataHead, char *command, int cummaCounter, int *dc,int line);

/*The operator of .data function
* Enters the data into the dataList
* and if label!=NULL it enters it to the labelsList
* return ERROR if have some error 
  else return TRUE 
*/
int dataF(char *command, char *label,int *dc, dataList **dataHead, labelsList **labelsHead,int line)
{
  int cummaCounter = 0, i = 0, currDc;
  currDc = *dc;
  if(!isEmpty(command))
  {
    printf("error:line %d; no arguments for .data function\n",line);
    return ERROR;
  }
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
    int num = addLabel(labelsHead,label,OFF,OFF,ON,OFF,currDc,line);
    if (!num)
    {
      return ERROR;
    }
    else
    {
      return TRUE;
    }
  }
  return TRUE;
}
/*The operator of .srting function
* Enters the data into the dataList add put in the end 0
* and if label!=NULL it enters it to the labelsList
* return ERROR if have some error 
* else return TRUE 
*/
int stringF(char *command, char *label, int *dc, dataList **dataHead, labelsList **labelsHead,int line)
{
  int currDc = *dc, i;
  int countr =0;
  int data;
    if(!isEmpty(command))
  {
    printf("Error:line %d: Missing arguments\n",line);
    return ERROR;
  }
  countr=countrChars(command, '\"',line);
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
    insertData(dataHead, data,*dc);
    (*dc)++;
    i++;
  }
  /*add a 0 in the and if the string*/
  insertData(dataHead, 0,*dc);
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
    int num = addLabel(labelsHead, label, OFF, OFF,ON,OFF,currDc,line);
    if (!num)
    {
      return ERROR;
    }
    else
    {
      return TRUE;
    }
  }
  return TRUE;
}
/*The operator of .mat function
*if declear on data it enters the data into the dataList
*else it put 0 (row*colume)tims in dataList
* and if label!=NULL it enters it to the labelsList
* return ERROR if have some error 
* else return TRUE 
*/
int matF(char *buff,char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line)
{

  int currDc = *dc, counter, matRow, matColum, matLen,diff;
  char openBracket='[',closeBracket=']';
int openCunter=0,closeCunter=0,i=0;
  if(!isEmpty(buff))
  {
    printf("Error:line %d: Missing arguments\n",line);
    return ERROR;
  }
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
            return TRUE;
        } 
    }
    i++;
}
if(closeCunter!=2 ||openCunter !=2)
{
printf("error:line %d: in .mat [num1][num2] format\n",line);
printf("mesing brackets !! \n");
 return ERROR;
}

  while (*buff != '[')
  {
    if (!isspace(*buff))
    {
printf("error:line %d: in .mat [num1][num2] format\n",line);
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
printf("error:line %d: in .mat [num1][num2] format\n",line);
      return ERROR;
    }
    buff++;
  }
  buff++;
   while (*buff != '['&&buff!=NULL)
  {
    if (!isspace(*buff))
    {
printf("error:line %d: in .mat [num1][num2] format\n",line);
      return ERROR;
    }
    buff++;
  }
  if (*buff != '[')
  {
    printf("error:line %d: in .mat [num1][num2] format\n",line);
    return ERROR;
  }
    buff++;
  matColum = (int)strtol(buff, &buff, 10);
  while (*buff != ']')
  {
    if (!isspace(*buff))
    {
      printf("error:line %d: in .mat [num1][num2] format\n",line);
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
    printf("error:line %d: in .mat [num1][num2] format\n",line);
    printf("num1 and num2 Must be greater than 0 n");
    return ERROR;
  }
  
  /*if No entries have been entered, we will save a space in datalist*/
/* TODO add test for chack if we have one variable*/
  if (counter == 0)
  {
    
    for (; matLen > 0; matLen--)
    {
      insertData(dataHead, 0,*dc);
      (*dc)++;
    }
  }
  else if(counter>=matLen)
  {
    printf("error:line %d: in .mat [num1][num2] format\n",line);
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
        insertData(dataHead, 0,*dc);
        (*dc)++;
      }
    }  

  /*add label if exzist one */
  if (label)
  {
    int num = addLabel(labelsHead,label,OFF,OFF,ON,OFF,currDc,line);
    if (!num)
    {
      return ERROR;
    }
    else
    {
      return NOT_EXIST;
    }
  }
  return NOT_EXIST;
} /*end of matF function*/


/*The operator of .extern function
* Enters the lebel into the labelsList
* return ERROR if have some error 
* else return TRUE 
*/
int externF(char *buff,char *label,labelsList **labelsHead,int line)
{
  if(!isEmpty(buff))
  {
    printf("Error:line %d: Missing arguments\n",line);
    return ERROR;
  }
  if(label)
  {
    printf("warnning:line %d:no need lable for .extern function.\n",line);
  }
  if(isDirectiveAddressing(buff,line)==VALID)
  {
    addLabel(labelsHead,buff,WAIT,ON,WAIT,OFF,0,line);
  }else
  {
    printf("error:line %d: unvalid label for extern function !!\n",line);
    return ERROR;
  }    
  return TRUE;
}/*end of externF function*/

/*The operator of .entry function
* Checks the integrity of the declared label
* return ERROR if have some error 
* else return TRUE 
*/
int entryF(char * buff,char *label,int line)
{
  int rval;
   if(!isEmpty(buff))
  {
    printf("Error:line %d: Missing arguments\n",line);
    return ERROR;
  }
    if(label)
          {
            printf("warnning:line %d:no need lable for .entry function.\n",line);
          }
 if((rval=isDirectiveAddressing(buff,line))==ERROR)
 {
  return ERROR;
 }
 else if(rval ==VALID)
 {
 return TRUE;
 }
 /*rval = NOT_EXIST*/
 printf("error : line %d: unligal label!!\n",line);
 return ERROR;
}/*end of entryF function*/

/*
An auxiliary function that count how mach time have a
spesipic char in a char arry 
Used in matF,dataF,stringF,
Returns
Error if there is any error.
True if there are no errors
*/
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
/*
An auxiliary function that takes a list of numbers
Separated by a comma and inserted into a datalist
Used in .mat and .data
Returns
Error if there is any error.
True if there are no errors
*/
int commaList(dataList **dataHead, char *command, int cummaCounter, int *dc,int line)
{
  char *cp;
  int data;
  /*get the data to the Datalist*/
  if(*command==',')
  {
    printf("error:line %d evgeny \n",line);
    return ERROR;
  }
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
        insertData(dataHead, data,*dc);
      *dc+=1;
      return TRUE;
    }
    
  }
  for (; cummaCounter >= 0; cummaCounter--)
  {
    if(!cp)
    {
      printf("Error:line %d; No  \n",line);/*TODO*/
      return ERROR;
    }else 
    {
      int i=0;
      while(isspace(cp[i])&&cp[i]!='\0'){i++;}
      if(cp[i]=='\0')
      {
        printf("error:line %d: no data between two commas \n ",line);/*TODO*/
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
      insertData(dataHead, data,*dc);
      *dc+=1;
    }
    cp = strtok(NULL, ",");
  }
  return TRUE;
}
