#include "consts.h"
#include "struct.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
int isComment(char * buff);
int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
int updateEntry(char *label,labelsList **labelsHead,int line)

int second_run((FILE *fp,labelsList **labelsHead,dataList **dataHead)
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
        int error =0,functionIndex;
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
        if(label!=NULL)/*we have a label!! :)*/
        {
          buff= strtok(NULL," ");/*we skipe to the next word*/    
        }
        if((functionIndex =isDirective(buff))!=NOT_EXIST)/*the function is directive stetment*/
        {
            if(functionIndex!=ENTRY)/*according to line 4 in th algoritem*/
            {
                lineCounter++;
                memset(line,'\0',LINE_LENGTH);
                continue;                
            }
            /*functionIndex=ENTRY*/
            buff= strtok(NULL,"\n");/*we get the rest of the line*/
            if(updateEntry(buff,labelsHead,lineCounter)==ERROR)
                errorFlag=ON;               
        } else if((functionIndex =isAction(buff))!=NOT_EXIST)/*the function is action stetment*/
        {
            if(decodeAction(functionIndex,lineCounter,)==ERROR)
                errorFlag=ON;                           
        }
        lineCounter++;
        memset(line,'\0',LINE_LENGTH);        
    }/*end of second big while*/
}   