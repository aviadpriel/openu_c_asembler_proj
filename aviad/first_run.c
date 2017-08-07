#include "struct.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define ERROR -1
#define LINE_LENGTH 81
/*evgeny : Declaration of functions that must be written (in another file)*/
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
line=(char *)malloc(sizeof(char)*LINE_LENGTH);
if(!line)
{
  printf("out of memory");
  exit(1);
}
  while(fgets( line, LINE_LENGTH,fp))
  {

    int error =0,functionIndex;
    char *buff;
    char *label;
	buff=strtok(line," ");
	label=isLabel(buff,&error,lineCounter);
    if(error ==ERROR)
    {
      errorFlag=ON;
    }else if(label!=NULL)/*we have a label!! :)*/
    {
      buff= strtok(NULL," ");/*we go to the next word*/
    }
    if((functionIndex =isDirective(buff))!=-2)/*the function is directive stetment*/
    {SWITCHER matrixF=OFF;
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
        case 0:/*.data function*/
        {
        if(dataF(buff,label,dc,dataHead,labelsHead,lineCounter)==ERROR)
          {errorFlag=ON;}

          break;        
        }
        case 1:/*.string function*/
        {
        if(stringF(buff,label,dc,dataHead,labelsHead,lineCounter)==ERROR)
          {errorFlag=ON;}
                    break;        
        }
        case 2:/*.mat function*/
        {
        if(matF(buff,label,dc,dataHead,labelsHead,lineCounter)==ERROR)
          {errorFlag=ON;}
          if(matrixF ==ON)
          {
            free(temp);
          }
                    break;        
        }
        case 3:/*.entry function*/
        {
          if(entryF(char * buff,char *label,int lineCounter)==ERROR)
          {errorFlag=ON;}
                    break;
        }
        case 4:/*.extern function*/
        {
          if(externF(buff,label,labelsHead,lineCounter)==ERROR)
          {errorFlag=ON;}
                    break;
        }
      }
    } else if((functionIndex =isAction(buff))!=-2)/*the function is action stetment*/
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
      printf("evgeny put a normal error messege: %s \n",buff);
    }

    lineCounter++;
    memset(line,'\0',LINE_LENGTH);
  }/*end of big while*/
free(line);
  if(errorFlag==OFF)
  {
	return 0;  
  }
  else
  return ERROR;
}

