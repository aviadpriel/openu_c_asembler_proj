#include <math.h>
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
            if(data<0||data>3)
            {
                printf("programing error in line %d",line);
                return ERROR;
            } 
            word->orgin=data;
         break;   
        }
        case 3:/*destintion*/
        {
            if(data<0||data>3)
            {
                printf("programing error in line %d",line);
                return ERROR;
            } 
            word->dest=data;
             break; 
                
         break;   
        }
        case 4:/*era*/
        {
            if(data<0||data>3)
            {
                printf("programing error in line %d",line);
                return ERROR;
            } 
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
            if(data<0||data>15)
            {
                printf("programing error in line %d",line);
                return ERROR;
            }
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
            printf("programing error in line %d",line);
            return ERROR;            
        }
    }
    return 0;

}

int decodeData(binWord * word,int data,int line)
{
    int mask2Bits=3,mask4Bits=15;
    if(data>=pow(2,WORD_SIZE) ||data<=-pow(2,WORD_SIZE))
    {
        printf("error:line %d : the number %d is bigger than %d size",line,data,WORD_SIZE);
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
void binWordToStrangeBase(binWord *addressWord,binWord *dataWord)
{
    int intAddress=0,intData=0;
    char *base4 ={"abcd"};
    char address[5]={"\0"};
    char data[6]={"\0"};    
   
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


    printf("%s \t %s \n",address,data);
}

