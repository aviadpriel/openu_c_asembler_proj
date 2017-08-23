#include "consts.h"
#include "struct.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

#define OB_SUFFIX ".ob"
#define ENT_SUFFIX ".ent"
#define EXT_SUFFIX ".ext"

/*פונקציות לזיהוי סוג הפונקציה*/
int isComment(char * buff);
int isEmpty(char * buff);
int isDirective(char *command);
int isAction(char *command);
/*פונקציה לזיהו תווית בתחילת שורה*/
char * isLabel(char * buf,int *error,int line);
/*סופר פונקציה שלב 2*/
int identification(char * buff,int functionIndex,int line,int *ic,SWITCHER secondRun,binWordList **binWordHead,labelsList **labelsHead,extEntList **extEntHead);
/*לעדכון תויות חצוניות */
int isInTheList(char *label,labelsList **labelsHead,int line,int *externalFlag,extEntList **extEntHead,int address);
/*פונקציות להדפסה */
/*תשתמש בהן כדי לצור את הפונקציות של הפלט*/
void printAndfree(FILE *ofp, binWordList *binWordHead);
void printAndfreeData(FILE *ofp, dataList *dataHead);

/*פונקצית עזר לקובץ זה בלבד*/
int updateEntry(char *label,labelsList **labelsHead,int line);
void modify_output_name(char *file_name, char *suffix, char **output_name);
void output_object(char *file_name, binWordList *binWordHead, dataList *dataHead, int IC, int DC);
void int_to_strange(FILE *ofp, int IC, int DC);
/*

*/
int second_run(FILE *fp,labelsList **labelsHead,dataList **dataHead, char *file_name, int DC, int IC)
{
    binWordList *binWordHead;
    extEntList *extEntHead;
    SWITCHER errorFlag = OFF; /*errorFlag check if we can to go to second_run */
    char *line;
    int lineCounter=1,ic2=INIT_IC;
    /*for print errors*/
    binWordHead=NULL;
    extEntHead=NULL;
    /*Confirming the cursor to the beginning of the file*/
    fseek(fp,0,SEEK_SET);
    line=(char *)malloc(sizeof(char)*LINE_LENGTH);
    if(!line)
    {
      printf("out of memory \n");
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
      /*ביצוע שיחרור זכרון*/
        return ERROR;
    }
    /*הוצעת קבצי פלט אם צריך*/
    output_object(file_name, binWordHead, *dataHead, IC, DC);
    /*if(extEntList)//creates .ent and .ext files if needed
    {

    }
    */
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
      break;
      }
    }
    labelsHead = &( (*labelsHead)->next);    
  }
  /*chack if the label is declear in the file */
  if(entryFlag==OFF)
  {
    printf("Error:line %d:the label ( %s ) is not decleared in the file\n",line,label);
    return ERROR;
  }
return 0;
}

/*****************************************************************************************************************************************************
	*Function:    void output_object(...)
  *Input:       char *file_name- the current file name to be assembeled.
                char *suffix- the suffix to current file name deppending on file creation type (.ob, .ext, .entry)
                char *output_name pointer which will point to relevant file name to be created.
	*Return:      pointer to relevant output file name.
	*Description: it takes the current file name and adds the relevant suffix for a new file to be created (.ob, .ext, .ent).
*****************************************************************************************************************************************************/
void output_object(char *file_name, binWordList *binWordHead, dataList *dataHead, int IC, int DC)
{
    char *output_name = NULL;
    FILE *ofp = NULL;
    modify_output_name(file_name, OB_SUFFIX, &output_name);
    ofp = fopen(output_name,"w");
    fprintf(ofp, "Base 4 \t Base 4 \nadress  machine-code\n");
    int_to_strange(ofp, IC - INIT_IC , DC);
    printAndfree(ofp, binWordHead);
    printAndfreeData(ofp, dataHead);
    free(output_name);
    fclose(ofp);
}

/*****************************************************************************************************************************************************
	*Function:    void output_object(...)
  *Input:       char *file_name- the current file name to be assembeled.
                char *suffix- the suffix to current file name deppending on file creation type (.ob, .ext, .entry)
                char *output_name pointer which will point to relevant file name to be created.
	*Return:      pointer to relevant output file name.
	*Description: it takes the current file name and adds the relevant suffix for a new file to be created (.ob, .ext, .ent).
*****************************************************************************************************************************************************/
/*void output_entry(char *file_name)
{
    char *output_name = NULL;
    modify_output_name(file_name, "ENT_SUFFIX", output_name)
    //creating 4 base output.
    FILE *fp = fopen(output_name,"w");
    free(output_name);
}*/

/*****************************************************************************************************************************************************
	*Function:    void output_object(...)
  *Input:       char *file_name- the current file name to be assembeled.
                char *suffix- the suffix to current file name deppending on file creation type (.ob, .ext, .entry)
                char *output_name pointer which will point to relevant file name to be created.
	*Return:      pointer to relevant output file name.
	*Description: it takes the current file name and adds the relevant suffix for a new file to be created (.ob, .ext, .ent).
*****************************************************************************************************************************************************/
/*void output_extern(char *file_name)
{
    char *output_name = NULL;
    modify_output_name(file_name, "EXT_SUFFIX", output_name)
    //creating 4 base output.
    FILE *fp = fopen(output_name,"w");
    free(output_name);
}*/

/*****************************************************************************************************************************************************
	*Function:    void modify_output_name(...)
    *Input:     char *file_name- the current file name to be assembeled.
                char *suffix- the suffix to current file name deppending on file creation type (.ob, .ext, .entry)
                char *output_name pointer which will point to relevant file name to be created.
	*Return:      pointer to relevant output file name.
	*Description: it takes the current file name and adds the relevant suffix for a new file to be created (.ob, .ext, .ent).
*****************************************************************************************************************************************************/
void modify_output_name(char *file_name, char *suffix, char **output_name)
{
    *output_name = (char*)malloc(sizeof(char) * (strlen(file_name) + strlen(suffix)));
    if(!file_name)
        {
            printf("memmory allocation failed\n");
            return;
        }
    strcpy(*output_name, file_name);
    strcat(*output_name, suffix);
}

/*int to strange base for IC\DC*/
void int_to_strange(FILE *ofp, int IC,int DC)
{
    char ic_adress[5];/*max memmory size is represented by 5 "digits"(256 == baaaa)*/
    char dc_adress[5];
	char base4[] = {"abcd"};
    int i = strlen(dc_adress) - 1;
    int j = strlen(dc_adress) - 1;
    ic_adress[5] = '\0';
    dc_adress[5] = '\0';
    if(!IC)/*case no memmory in usage*/
        ic_adress[i--] = 'a';
	while(i >= 0 && IC)
	{
		ic_adress[i] = base4[IC % 4];
		IC /= 4;
		i--;
    }
    if(!DC)/*case no memmory in usage*/
    dc_adress[i--] = 'a';
    while(j >= 0 && DC)
	{
		dc_adress[j] = base4[DC % 4];
		DC /= 4;
		j--;
    }
    i++;
    j++;
    fprintf(ofp, " %s \t %s\n\n", (ic_adress + i), (dc_adress + j));
}