#include "consts.h"
#include "struct.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

void catBinWordList(binWordList **binWordHead,binWordList **binWordBuff,int curIC);
int countrChars(char * word, char c, int line);
int isComment(char * buff);
int isEmpty(char * buff);
int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
int isImmediateAddressing(char * buff, int line);
int isDirectiveAddressing(char * buff, int line);
int isMatrix(char *buff,int line,int *r1,int *r2);
int setBits(int data,char * type,binWord * word,int line);
int updateEntry(char *label,labelsList **labelsHead,int line);
void initWord(binWord *word);
int binWordToInt(binWord *word);
void addBinWord(binWordList **binWordHead,binWord *word,int address);
void binWordToStrangeBase(binWord *addressWord,binWord *dataWord);
void printAndfree(binWordList *binWordHead);
void printAndfreeData(dataList *dataHead);
char * isLabel(char * buf,int *error,int line);
int identification(char * buff,int functionIndex,int line,int *ic,SWITCHER secondRun,binWordList **binWordHead,labelsList **labelsHead,extEntList **extEntHead);
void addExtEnt(extEntList **extEntHead,char *label,int address,DIRECRIVE_FUNCTION type);
int isInTheList(char *label,labelsList **labelsHead,int line,int *externalFlag,extEntList **extEntHead,int address);

int second_run(FILE *fp,labelsList **labelsHead,dataList **dataHead, char *file_name)
{
    binWordList *binWordHead;
    extEntList *extEntHead;
    SWITCHER errorFlag = OFF; /*errorFlag check if we can to go to second_run */
    char *line;
    int lineCounter=1,ic2=INIT_IC;
    /*for print errors*/
    /*Confirming the cursor to the beginning of the file*/
    binWordHead=NULL;
    extEntHead=NULL;
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
            isInTheList(label,labelsHead,lineCounter,NULL,&extEntHead,ic2);            
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
            buff= strtok(NULL,"\n");/*we get the rest of the line*/            
            if(identification(buff,functionIndex,lineCounter,&ic2,ON,&binWordHead,labelsHead,&extEntHead)==ERROR)
                errorFlag=ON;                           
        }
        lineCounter++;
        memset(line,'\0',LINE_LENGTH);        
    }/*end of second big while*/
    if(errorFlag==ON)
    {
        return ERROR;
    }
    printAndfree(binWordHead);
    printAndfreeData(*dataHead);
    return 0;
}


int updateEntry(char *label,labelsList **labelsHead,int line)
{
  SWITCHER entryFlag=OFF;
  int i=0,functionLen;
  while(isspace(*label)){label++;}
  while(!isspace(label[i])&&label[i]!='\0'){i++;}
  functionLen = i;
  /* insert using a loop and pointer to pointer*/
  while(*labelsHead)
  {
    if(strncmp(label,(*labelsHead)->label,strlen((*labelsHead)->label))==0)/*need to update the entry flag */
    {
      if(functionLen==strlen((*labelsHead)->label))
      {
      (*labelsHead)->entry=ON;
      entryFlag=ON;
      }
    }
    labelsHead = &( (*labelsHead)->next);    
  }
  /*chack if the label is declear in the file */
  if(entryFlag==OFF)
  {
    printf("error:line %d:the label ( %s ) is not decleared in the file\n",line,label);
    return ERROR;
  }
return 0;
}