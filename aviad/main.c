#include <math.h>
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct binWord{
   unsigned int opcode:4;
   unsigned int orgin:2;
   unsigned int dest:2;
   unsigned int era:2;
    }binWord;
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
    {"are",0,1},
    {"addres",2,9},
    {"immediate",2,9},
    {"opcode",6,9},    
    {"orginReg",6,9},
    {"destReg",2,5},
    {NULL,ERROR,ERROR}};     

int first_run(FILE *fp,labelsList **labelsHead,dataList **dataHead,int *dc,int *ic);
/***************/
int decodeData(binWord * word,int data,int line);
void setBitsAddresOrImmediate(binWord * word,int data);
/***************************/
int main()
{
binWord test;
test.opcode=-2;
printf(" test.opcode = %d test.orgin = %d \n",test.opcode ,test.orgin);
return 0;
    
    /*
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
freeDataList(dataHead);
freeLabelsList(labelsHead);
return 0; */
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
    int mask2Bits = 2,mask4Bits=15;
    word->dest=data&mask2Bits;
    data >>=2;
    word->orgin=data&mask2Bits;
    data >>=2;
    word->opcode=data&mask4Bits;
}


