#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<ctype.h>
#define WORD_SIZE 10
#define ERROR -1
#define LABEL_MAX_LEN 30


typedef enum {ON,OFF,WAIT} SWITCHER;
typedef struct bitType{

  char * type;
  int startBit;
  int endBit;
}bitType;

typedef struct binWord{
  int opcode:4;
  int orgin:2;
  int dest:2;
  int era:2;
  }binWord; 

  typedef struct binWordList{
    binWord word;
    struct binWordList *next;
}binWordList;

typedef struct labelsList {
  char *label;
SWITCHER entry;
SWITCHER data;  
SWITCHER action;
SWITCHER external;
int address;
struct labelsList *next;
} labelsList;
typedef struct dataList{
int data:WORD_SIZE;
struct dataList *next;
}dataList;
typedef struct commandList{
char *name;  
int operends;
int operendGroup;
}commandList;

dataList* newData(int data);

/************dataList functions*******/
dataList* newData(int data)
{
  dataList *p = (dataList*)malloc(sizeof(dataList));
  if(!p)
  {
    printf("mermory alloction error\n");
    exit(1);
  }
  p->data = data;
  p->next=NULL;
  return p;
}


void insertData(dataList **dataHead,int data)
{
   dataList *current=(*dataHead);
  if(!(*dataHead))
  {
  (*dataHead)=newData(data);

  }
  else
    {
    while (current->next != NULL) {
        current = current->next;
    }
    current->next=newData(data);
    }
}


/************labelsList functions*******/
labelsList* newLabel(char *label,int address,SWITCHER action,SWITCHER external,SWITCHER data,SWITCHER entry)
{
  labelsList *p = (labelsList*)malloc(sizeof(labelsList));
  if (!p) {
    printf("memmory error exit\n");
    exit(1);
  }
  p->label=(char *)malloc(sizeof(char)*LABEL_MAX_LEN);
  strncpy(p->label,label,LABEL_MAX_LEN);
  p->action=action;
  p->external=external;
  p->address=address;
  p->data=data;
  p->entry=entry;
  p->next =NULL;
  return p;
}

int addLabel(labelsList **labelsHead,char *label,SWITCHER action,SWITCHER external,SWITCHER data,SWITCHER entry,int address,int line)
{
/* insert using a loop and pointer to pointer*/
  while(*labelsHead)
  {
     if(strcmp(label,(*labelsHead)->label)!=0)
       labelsHead = &( (*labelsHead)->next);
     else/*error the label is in the list */
    {
      printf("Error: line %d:the label %s is in the list \n",line,label);
      return ERROR;
    }
  }
  *labelsHead = newLabel(label,address,action,external,data,entry);
  return 1;
}

void freeDataList(dataList *dataHead)
{
  dataList* temp;
    while (dataHead != NULL)
    { 
        temp = dataHead; 
        temp ->next=NULL;
        dataHead = dataHead->next;
        free(temp);
    }
}
void freeLabelsList(labelsList *labelsHead)
{
  labelsList* temp;
    while (labelsHead != NULL)
    { 
        temp = labelsHead; 
        temp ->next=NULL;
        labelsHead = labelsHead->next;
        free(temp);
    }
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

binWordList* newBinWord(binWord word)
{
  binWordList *p = (binWordList*)malloc(sizeof(binWordList));
  if (!p) {
    printf("memmory error exit\n");
    exit(1);
  }
  p->word.opcode=word.opcode;
  p->word.orgin=word.orgin;
  p->word.dest=word.dest;
  p->word.era=word.era;
  p->next=NULL;
  return p;
}
void addBinWord(binWordList **binWordHead,binWord word)
{
  while(*binWordHead)
  {
    binWordHead = &( (*binWordHead)->next);
  }
*binWordHead = newBinWord(word);
}
void catBinWordList(binWordList **binWordHead,binWordList **binWordBuff)
{
  while(*binWordHead)
  {
    binWordHead = &( (*binWordHead)->next);
  }
  *binWordHead=*binWordBuff;
}
