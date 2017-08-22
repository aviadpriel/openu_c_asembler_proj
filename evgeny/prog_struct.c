#include "consts.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<ctype.h>

#define OPCODE_SIZE 4
#define ORGIN_SIZE 2
#define DEST_SIZE 2
#define ERA_SIZE 2
/*for bin word*/
typedef struct bitType{
  char * type;
  int startBit;
  int endBit;
  }bitType;
  static bitType _bitType[]={
  {"number",0,9},
  {"char",0,9},
  {"orgin",4,5},
  {"destin",2,3},
  {"era",0,1},
  {"addres",2,9},
  {"immediate",2,9},
  {"opcode",6,9},    
  {"orginReg",6,9},
  {"destReg",2,5},
  {NULL,ERROR,ERROR}};     

  #ifndef SWITCHER_1
  #define SWITCHER_1
  typedef enum {ON,OFF,WAIT} SWITCHER;
  typedef enum {DATA=0,STRING=1,MATRIXF=2,ENTRY=3,EXTERN=4} DIRECRIVE_FUNCTION;
  #endif
  
typedef struct extEntList{
  char *label;
  int address;
  DIRECRIVE_FUNCTION type;
  struct extEntList *next;
}extEntList;
void addExtEnt(extEntList **extEntHead,char *label,int address,DIRECRIVE_FUNCTION type);/*נשארת פה*/

/*מייצג מילה בגודל 10*/
typedef struct binWord{
 unsigned int opcode:OPCODE_SIZE;
 unsigned int orgin:ORGIN_SIZE;
 unsigned int dest:DEST_SIZE;
 unsigned int era:ERA_SIZE;
  }binWord; 

  typedef struct binWordList{
    binWord word;
    int address;    
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
int address;
struct dataList *next;
}dataList;


/************dataList functions*******/
dataList* newData(int data,int address)
{
  dataList *p = (dataList*)malloc(sizeof(dataList));
  if(!p)
  {
    printf("mermory alloction error\n");
    exit(1);
  }
  p->data = data;
  p->address=address;
  p->next=NULL;
  return p;
}


void insertData(dataList **dataHead,int data,int address)
{
   dataList *current=(*dataHead);
  if(!(*dataHead))
  {
  (*dataHead)=newData(data,address);
  }
  else
    {
    while (current->next != NULL) {
        current = current->next;
    }
    current->next=newData(data,address);
    }
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

void updateDataList(dataList **dataHead,int curIc)
{
  while(*dataHead)
  {
      (*dataHead)->address+=curIc;
      dataHead = &( (*dataHead)->next);    
      
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
  return 0;
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

/**********************************/

/* binWord functions   */

binWordList* newBinWord(binWord word,int address)
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
  p->address=address;
  p->next=NULL;
  return p;
}
void addBinWord(binWordList **binWordHead,binWord *word,int address)
{
  while(*binWordHead)
  {
    binWordHead = &( (*binWordHead)->next);
  }
  *binWordHead = newBinWord(*word,address);
}

int isInTheList(char *label,labelsList **labelsHead,int line,int *externalFlag,extEntList **extEntHead,int address)
{
   int i=0,functionLen;
   if(externalFlag)
   {
   *externalFlag=0;
   }   
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
          if((*labelsHead)->external==ON)
          {
            addExtEnt(extEntHead,(*labelsHead)->label,address,EXTERN);            
            if(externalFlag)
            {
            *externalFlag=1;
            }
            

          }else if((*labelsHead)->entry==ON)
          {
            if(externalFlag)
            {
              addExtEnt(extEntHead,(*labelsHead)->label,address,ENTRY);                 
            }
            else
            {
              addExtEnt(extEntHead,(*labelsHead)->label,(*labelsHead)->address,ENTRY);                 
            }
          }
       return (*labelsHead)->address;
       }
     }
     labelsHead = &( (*labelsHead)->next);    
   }
 
       printf("error:line %d:the label in the function not declir enywear !!\n",line);
           return ERROR;
   
}

void initWord(binWord *word)
{
   word->opcode=word->orgin=word->dest=word->era=0;      
}


int decodeData(binWord * word,int data,int line)
{
    int mask2Bits=3,mask4Bits=15;
    if(data>=pow(2,WORD_SIZE) ||data<=-pow(2,WORD_SIZE))
    {
        printf("Error:line %d : the number %d is bigger than %d size",line,data,WORD_SIZE);
        return ERROR;
    }
    word->era=data&mask2Bits;
    data>>=2;
    word->dest=data&mask2Bits;
    data>>=2;    
    word->orgin=data&mask2Bits;
    data>>=2;
    word->opcode=data&mask4Bits;
    return 0;

}

void setBitsAddresOrImmediate(binWord * word,int data)
{
    int mask2Bits = 3,mask4Bits=15;
    word->dest=data&mask2Bits;
    data >>=2;
    word->orgin=data&mask2Bits;
    data >>=2;
    word->opcode=data&mask4Bits;
}

/*no need for aviad*/
int binWordToInt(binWord *word)
{
    int intNum=0;
    intNum|=word->opcode;
    intNum<<=2;
    intNum|=word->orgin;
    intNum<<=2;
    intNum|=word->dest;
    intNum<<=2;
    intNum|=word->era;
    return intNum;
}


int setBits(int data,char * type,binWord * word,int line)
{
    int i;
    for(i=0;_bitType[i].type!=NULL;i++)
    {
        if(!strcmp(_bitType[i].type,type))
        break;
    }
    switch(i)
    {
        case 0:/*number*/
        case 1:/*char*/
        {
        if (decodeData(word,data,line)==ERROR)
            return ERROR;
         break;   
        }
        case 2:/*orgin*/
        {
         word->orgin=data;
         break;   
        }
        case 3:/*destintion*/
        {
            word->dest=data;
             break; 
                
         break;   
        }
        case 4:/*era*/
        {
            word->era=data;
            break;  
        }
        case 5:/*addres*/
        case 6:/*immediate*/
        {
            setBitsAddresOrImmediate(word,data); 
         break;   
        }
        case 7:/*opcode*/
        case 8:/*orginReg*/
        {
            word->opcode=data; 
            break;            
        }
        case 9:/*destReg*/
        {
            int mask2Bits =3;
            word->dest=data&mask2Bits;
            data>>=2;
            word->orgin=data&mask2Bits;            
            break;
        }
        default :
        {
            return ERROR;            
        }
    }
    return 0;

}

void binWordToStrangeBase(FILE *fp, binWord *addressWord,binWord *dataWord)
{
    int intAddress=0,intData=0;
    char *base4 ={"abcd"};
    char address[5]={"\0"};
    char data[6]={"\0"};    
   
    /*decode the address word*/
    intAddress=addressWord->opcode;
    intAddress&=3;
    address[0]=base4[intAddress];    
    address[1]=base4[addressWord->orgin];
    address[2]=base4[addressWord->dest];
    address[3]=base4[addressWord->era];

    /*decode the data word*/
    intData=dataWord->opcode;
    intData>>=2;
    data[0]=base4[intData];
    intData=dataWord->opcode;
    intData&=3;
    data[1]=base4[intData];
    data[2]=base4[dataWord->orgin];
    data[3]=base4[dataWord->dest];
    data[4]=base4[dataWord->era];

      /*print to the screen*/
    fprintf(fp, "%s \t %s \n",address,data);
}

void printAndfree(FILE *fp, binWordList *binWordHead)
{
   binWordList* temp;
   binWord addressWord;
       while (binWordHead != NULL)
       { 
           initWord(&addressWord);            
           temp = binWordHead; 
           binWordHead = binWordHead->next;            
           temp ->next=NULL;
           setBits(temp->address,"number",&addressWord,0);                    
           binWordToStrangeBase(fp, &addressWord,&(temp->word));
           free(temp);
       }
   
}


void printAndfreeData(FILE *fp, dataList *dataHead)
{
   binWord dataWord,addressWord;
   dataList *temp;
   while (dataHead != NULL)
   { 
       unsigned int data,address;
       initWord(&dataWord);
       initWord(&addressWord);
       
       temp = dataHead; 
       dataHead = dataHead->next; 
       temp ->next=NULL;        
       data=temp->data;
       address=temp->address;
       setBits(data,"number",&dataWord,0);
       setBits(address,"number",&addressWord,0);        
       binWordToStrangeBase(fp, &addressWord,&dataWord);
       free(temp);
   }

}

extEntList* newExtEnt(char *label,int address,DIRECRIVE_FUNCTION type)
{
  extEntList *p = (extEntList*)malloc(sizeof(extEntList));
  if (!p) {
    printf("memmory error exit\n");
    exit(1);
  }
  if(label)
  {
  p->label=(char*)malloc(sizeof(char)*strlen(label));
  memset(p->label,'\0',strlen(label));
  strcpy(p->label,label);
  }
  else{
  p->label=NULL;
  }
  p->address=address;
  p->type=type;
  p->next=NULL;
  return p;
}

void addExtEnt(extEntList **extEntHead,char *label,int address,DIRECRIVE_FUNCTION type)
{
  while(*extEntHead)
  {
    extEntHead = &( (*extEntHead)->next);
  }
*extEntHead = newExtEnt(label,address,type);
}


void catExtEntList(extEntList **extEntHead,extEntList **extEntBuff,int ic)
{
  extEntList *temp;
  while(*extEntHead)
  {
    extEntHead = &( (*extEntHead)->next);
  }
  /* *extEntHead = null*/
  while(*extEntBuff)
  {
    if((*extEntBuff)->label)
    {
      *extEntHead = *extEntBuff;
      extEntBuff = &( (*extEntBuff)->next);
      (*extEntHead)->next=NULL;
      (*extEntHead)->address+=ic+1;
    }else
    {
      temp=*extEntBuff;
      extEntBuff = &( (*extEntBuff)->next);
      free(temp);      
    }
  }
}

