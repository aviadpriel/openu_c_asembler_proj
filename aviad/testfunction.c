#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR -1
int countrChars(char *word, char c);
/*get numbers from comma list like 2,3,6,4,8,9*/
int commaList(dataList **dataHead, char *command, int cummaCounter, int *dc);
int isGuidelineStatement(char *p);
typedef struct commends
{
  char *command;
} commends;
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
int isGuidelineStatement(char *p)
{
  int i;
  commends cmd[] = {{".data"}, {".string"}, {".mat"}, {".entry"}, {".extern"}};
  for (i = 0; i < 5; i++)
  {
    if ((strcmp(cmd[i].command, p)) == 0)
      return i;
  }
  return -2;
}

void guidelineStatement(char *commend, int commandIndex, int dc)
{
  switch (commandIndex)
  {
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

/* .data 5,2,6,7*/
int dataF(char *command, char *label, SWITCHER lableFlag, int *dc, dataList **dataHead, labelsList **labelsHead)
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
        printf("error:double commas ");
        return ERROR;
      }
    }
    i++;
  }

  if ((commaList(dataHead, command, cummaCounter, dc)) == ERROR)
    return ERROR;
  if (lableFlag == ON)
  {
    int num = addLabel(labelsHead, label, OFF, OFF, currDc);
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
int stringF(char *command, char *label, SWITCHER lableFlag, int *dc, dataList **dataHead, labelsList **labelsHead)
{
  int currDc = *dc, i;
  int countr = countrChars(command, '\"');
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
      printf("error in format of .string \"somestring\"  %s\n", command);
      return ERROR;
    }
    i++;
  }
  i++; /*for pass the " char*/
  while ((data = command[i]) != '\"')
  {
    /*
        if(!isalnum(data)&&(!isspace(data)))
        {

      }
      */
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
      printf("error in format of .string \"somestring\"  %s\n", command);
      return ERROR;
    }
    i++;
  }
  /*and Check... */

  /*add label if exzist one */
  if (lableFlag == ON)
  {
    int num = addLabel(labelsHead, label, OFF, OFF, currDc);
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

int matF(char *command, char *label, SWITCHER lableFlag, int *dc, dataList **dataHead, labelsList **labelsHead)
{

  int currDc = *dc, counter, matRow, matColum, matLen,diff;
  counter = countrChars(command, '[');
  if (counter != 2)
  {
    printf("error: in  .mat [num1][num2] format: .mat \n");
    return ERROR;
  }
  counter = countrChars(command, ']');
  if (counter != 2)
  {
    printf("error: in .mat [num1][num2] format: .mat \n");
    return ERROR;
  }

  while (*command != '[')
  {
    if (!isspace(*command))
    {
      printf("error: in .mat [num1][num2] format: .mat n");
      return ERROR;
    }
    command++;
  }
  /*We will examine the first casings*/
  command++;
  matRow = (int)strtol(command, &command, 10);
  while (*command != ']')
  {
    if (!isspace(*command))
    {
      printf("error: in .mat [num1][num2] format: .mat \n");
      return ERROR;
    }
    command++;
  }
  command++;
  if (*command != '[')
  {
    printf("error: in .mat [num1][num2] format: .mat \n");
    return ERROR;
  }
    command++;
  matColum = (int)strtol(command, &command, 10);
  while (*command != ']')
  {
    if (!isspace(*command))
    {
      printf("errsor: in .mat [num1][num2] format: .mat \n");
      return ERROR;
    }
    command++;
  }
   command++;
  counter = countrChars(command, ',');
  if (counter == ERROR)
    return ERROR;

  matLen = matColum * matRow;
  if (!matLen)
  {
    printf("error: in .mat [num1][num2] format:\n");
    printf("num1 and num2 Must be greater than 0 n");
    return ERROR;
  }
  /*if No entries have been entered, we will save a space in datalist*/

  if (counter == 0)
  {
    for (; matLen > 0; matLen--)
    {
      insertData(dataHead, 0);
      (*dc)++;
    }
  }
  else
  {
    if ((commaList(dataHead, command, counter, dc)) == ERROR)
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
  if (lableFlag == ON)
  {
    int num = addLabel(labelsHead, label, OFF, OFF, currDc);
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

int countrChars(char *word, char c)
{
  int counter = 0;
  while (*word != '\0')
  {
    if (*word == c)
    {
      word++;
      counter++;
      if (*word == c)
      {
        printf("error:duble %c is unligal!!\n", c);
        return ERROR;
      }
    }
    word++;
  } /*end of while*/
  return counter;
}
int commaList(dataList **dataHead, char *command, int cummaCounter, int *dc)
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
      printf("un prespectiv varible!! %c \n", *cp);
      return ERROR;
    }
  }
  for (; cummaCounter >= 0; cummaCounter--)
  {
    data = (int)strtol(cp, &cp, 10);
    /*test if cp is empty*/
    while (*cp == ' ')
    {
      cp++;
    }
    if (*cp != '\0')
    {
      printf("un prespectiv varible!! %c \n", *cp);
      return ERROR;
    }
    else
    {
      insertData(dataHead, data);
      (*dc)++;
    }
    cp = strtok(NULL, ",");
  }
  return 0;
}
