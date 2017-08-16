#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int first_run(FILE *fp,labelsList **labelsHead,dataList **dataHead,int *dc,int *ic);
int second_run(FILE *fp,labelsList **labelsHead,dataList **dataHead, char *file_name);
/***************/
int decodeData(binWord * word,int data,int line);
int setBits(int data,char * type,binWord * word,int line);
void setBitsAddresOrImmediate(binWord * word,int data);
int binWordToInt(binWord *word);
void binWordToStrangeBase(binWord *addressWord,binWord *dataWord);
void initWord(binWord * word);
/***************************/
void updateEntry(char *label,labelsList **labelsHead,int line);
int main()
{
labelsList *labelsHead=NULL;

dataList *dataHead=NULL;
int ic=100,dc=0,rval=0;
FILE *fp;
fp=fopen("test.as","r");
if(!fp)
{
printf("cannot open file!! \n");
exit(1);
}


rval=first_run(fp,&labelsHead,&dataHead,&dc,&ic);
printf("ic is :%d \n dc is :%d \n rval is :%d \n",ic,dc,rval);
if(!rval)
{
    second_run(fp, &labelsHead, &dataHead, NULL);    
}
freeDataList(dataHead);
freeLabelsList(labelsHead);
return 0; 
}
/*
int main()
{
    binWord word;
    initWord(&word);
    word.era=2;
    setBits(133,"addres",&word,0);
    binWordToStrangeBase(&word);
    return 0;

}*/

