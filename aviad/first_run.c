/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : first_run.c
 Semester    : 2017b
 Description : the function which runs the first part of the algorithm
 ============================================================================
pasado-code of function first_run
1. If a null is returned, go to 8
2. If it is an empty row or a note line, return to 1
3. Take a word from the line
4. If it is a label save it and go to the next word
5. If this is the directive function, do related actions to the directive function and return to 1
6. If this is an action function, do related actions to the action function and return to 1
7. An error message has been sent: Unrecognized / valid expression, return to 1
8. If an error has occurred so far, return ERROR another return 0

Discounts
1. The maximum length of the line is 80

Remarks
1. The tests of the guidance functions are performed in the DirectiveTest.c
  2. Tests of the actions functions are performed in 
3. Related operations = (in the health + housing + assignment in data structures + calculation of ic / dc)
*/
#include "consts.h"
#include "struct.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
/*evgeny : Declaration of functions that must be written (in another file)*/
int isComment(char * buff);
int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
int identification(char * buff,int functionIndex,int line,int *ic);
int dataF(char *command, char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line);
int stringF(char *command,char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line);
int matF(char *command,char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line);
int externF(char *buff,char *label,labelsList **labelsHead,int line);
int entryF(char * buff,char *label,int line);

char * isLabel(char * buf,int *error,int line);

int first_run(FILE *fp,labelsList **labelsHead,dataList **dataHead,int *dc,int *ic)
{

  SWITCHER errorFlag = OFF; /*errorFlag check if we can to go to second_run */
  char *line;
  int lineCounter=1 ;/*for print errors*/
  /*Confirming the cursor to the beginning of the file*/
  fseek(fp,0,SEEK_SET);
  *ic=INIT_IC;
  line=(char *)malloc(sizeof(char)*LINE_LENGTH);
  if(!line)
  {
    printf("out of memory");
    exit(1);
  }
  while(fgets( line, LINE_LENGTH,fp))
  {

    int error =TRUE,functionIndex;
    char *buff;
    char *label;
    buff=strtok(line," ");
    /*chack if line is comment or a empty line*/
  if(!isComment(buff)||*buff=='\n')
  {
    lineCounter++;
    memset(line,'\0',LINE_LENGTH);
      continue;/*no need to do the other tests*/
  }
	label=isLabel(buff,&error,lineCounter);
    if(error ==ERROR)
    {
      errorFlag=ON;
      lineCounter++;
      memset(line,'\0',LINE_LENGTH);
      continue;/*no need to do the other tests*/
    }else if(label!=NULL)/*we have a label!! :)*/
    {
      buff= strtok(NULL," ");/*we go to the next word*/
      if(*buff=='\n'||buff==NULL)
      {
      printf("error:line %d:cannot add a label wite a empty line\n",lineCounter);/*evgeny corrct me!!!!*/
      errorFlag=ON;
      lineCounter++;
      memset(line,'\0',LINE_LENGTH);
      continue;/*no need to do the other tests*/
      }
    }
    if((functionIndex =isDirective(buff))!=-2)/*the function is directive stetment*/
    {
      SWITCHER matrixF=OFF;
      char *temp=NULL;
      if(functionIndex==2)
      {
        int i=0;
        while(buff[i]!='['&&buff[i]!='\0'){i++;}
        if(buff[i]=='[')
        {
          buff+=i;
          temp=(char *)malloc(sizeof(char)*(strlen(buff)));
          strcpy(temp,buff);
          matrixF=ON;
       }  
      }
      buff= strtok(NULL,"\n");/*we get the rest of the line*/
      if(matrixF==ON)
      {
        temp=(char *)realloc(temp,sizeof(char)*(strlen(buff)+strlen(temp)));
        strcat(temp,buff);
        buff=temp;
      }
      switch(functionIndex)
      {
        case DATA:/*.data function*/
        {
        if(dataF(buff,label,dc,dataHead,labelsHead,lineCounter)==ERROR)
            {errorFlag=ON;}

          break;        
        }
        case STRING:/*.string function*/
        {
        if(stringF(buff,label,dc,dataHead,labelsHead,lineCounter)==ERROR)
            {errorFlag=ON;}
                    break;        
        }
        case MATRIXF:/*.mat function*/
        {
        if(matF(buff,label,dc,dataHead,labelsHead,lineCounter)==ERROR)
            {errorFlag=ON;}
            if(matrixF ==ON)
            {
              free(temp);
            }
                    break;        
        }
        case ENTRY:/*.entry function*/
        {
          if(entryF(buff,label,lineCounter)==ERROR)
          {errorFlag=ON;}
                    break;
        }
        case EXTERN:/*.extern function*/
        {
          if(externF(buff,label,labelsHead,lineCounter)==ERROR)
          {errorFlag=ON;}
                    break;
        }
      }
    } else if((functionIndex =isAction(buff))!=NOT_EXIST)/*the function is action stetment*/
    {
      int curIc=*ic;
      buff= strtok(NULL,"\n");/*we get the rest of the line*/
	    if(identification(buff,functionIndex,lineCounter,ic)==ERROR)
	    {
		      errorFlag=ON;
	    }else if(label)
      {
        if(addLabel(labelsHead,label,ON,OFF,OFF,curIc,lineCounter)==ERROR)
          		errorFlag=ON;
      }
    }
    else /*unligal stetment print error*/
    {
      errorFlag=ON;      
      printf("evgeny put a normal error messege: %s \n",buff);
    }

    lineCounter++;
    memset(line,'\0',LINE_LENGTH);
  }/*end of big while*/
free(line);

if(*dc==0 && *ic==INIT_IC)/*chack if is no a empty file */
  {
    printf("this is a emty file \n");
      return ERROR;
  } 
  else if(errorFlag== OFF)
  {
	  return TRUE;  
  }
  return ERROR;
}

