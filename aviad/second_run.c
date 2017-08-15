#include "consts.h"
#include "struct.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

void catBinWordList(binWordList **binWordHead,binWordList **binWordBuff);
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
int isInTheList(char *label,labelsList **labelsHead,int line);
void initWord(binWord *word);
void addBinWord(binWordList **binWordHead,binWord word);
char * isLabel(char * buf,int *error,int line);
int identifications(char * buff,int functionIndex,int line,int *ic,SWITCHER secondRun,binWordList **binWordHead,labelsList **labelsHead);

static commandList _action[]={{"mov",2,4,3},
{"cmp",2,4,4},
{"add",2,4,3},
{"sub",2,4,3},
{"lea",2,2,3},
{"not",1,0,3},
{"clr",1,0,3},
{"inc",1,0,3},
{"dec",1,0,3},
{"jmp",1,0,3},
{"bne",1,0,3},
{"red",1,0,3},
{"prn",1,0,4},
{"jsr",1,0,3},
{"rts",0,0,0},
{"stop",0,0,0},
{NULL,0,0,0}};

int second_run(FILE *fp,labelsList **labelsHead,dataList **dataHead, char *file_name)
{
    binWordList *binWordHead;
    SWITCHER errorFlag = OFF; /*errorFlag check if we can to go to second_run */
    char *line;
    int lineCounter=1,ic2=INIT_IC;
    /*for print errors*/
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
            buff= strtok(NULL,"\n");/*we get the rest of the line*/            
            if(identifications(buff,functionIndex,lineCounter,&ic2,ON,&binWordHead,labelsHead)==ERROR)
                errorFlag=ON;                           
        }
        lineCounter++;
        memset(line,'\0',LINE_LENGTH);        
    }/*end of second big while*/
    if(errorFlag==ON)
    {
        return ERROR;
    }
    return 0;
}
/*pasdo-code for crazy decodeAction
*1.idenfy the addressing methds
*2.idenfy cost
*3.idenfy data and find addres of labels
*4 if have errors stop
*5 decod the words in binWordList;
*6 learn to infi 2
*/

int identifications(char * buff,int functionIndex,int line,int *ic,SWITCHER secondRun,binWordList **binWordHead,labelsList **labelsHead)
{    
binWord word;
addressing_method firstOp,secondOp;    
int firstCost=0,secondCost=0,rval=0,r1=0,r2=0;
int cammaCounter;
/*for second run*/
initWord(&word);
if(buff)
{
    cammaCounter=countrChars(buff,',',line);
    if(cammaCounter==ERROR)
    {
        return ERROR;
    }
} else if(_action[functionIndex].operends!=0)
    {
        printf("error:line %d:no operends for \"%s\" \n",line,_action[functionIndex].name);
        return ERROR;
    
    }

if(_action[functionIndex].operends==0)/*if no operends*/
{
    if(secondRun==ON)
    {
    setBits(functionIndex,"opcode",&word,line);
    setBits(0,"orgin",&word,line);
    setBits(0,"destin",&word,line);
    setBits(0,"era",&word,line);
    addBinWord(binWordHead,word);
    return 0;    
    }
    if(buff==NULL)
    {
        *ic+=1;/*for PSW*/
        return 0;
    }

 while(!isspace(*buff)){buff++;}
 if(buff!=NULL)
 {
   printf("no need operends for %s function.\n",_action[functionIndex].name);  
   return ERROR;  
 }
    *ic+=1;/*for PSW*/
    return 0;
}else if(_action[functionIndex].operends==1)
{
    if(cammaCounter>0)
    {
        printf ("ERROR:line %d:implicit declaration of function ‘%s’ \n",line,_action[functionIndex].name);
        return ERROR;
    }
    if(strcmp("prn",_action[functionIndex].name)==0)
    {

        rval=isImmediateAddressing(buff,line);
        if(rval==VALID)
        {
            if(secondRun==ON)
            {
                int intData;
                char asterisk='#';
                binWord data;
                initWord(&data);
                while(*buff!=asterisk){buff++;}
                buff++;/*skip the asterisk */
                intData=strtol(buff,&buff,10);
                /*decod the first word*/
                /*no need to decod the others*/
                setBits(functionIndex,"opcode",&word,line);
            
                /*decod the second word*/                
                if(setBits(intData,"immediate",&data,line)==ERROR)
                   { return ERROR;}

                addBinWord(binWordHead,word);
                addBinWord(binWordHead,data);                  
            }
            *ic+=2;
            return 0; 
        }else if(rval == ERROR)
            {
                return ERROR;
            }
    }
    rval=isRegister(buff);
    if(rval!=NOT_EXIST)
    {
        if(secondRun==ON)
        {
            /*decode the register !!*/
            binWord regWord;
            regWord.opcode=regWord.orgin=regWord.dest=regWord.era=0;            
            
            setBits(functionIndex,"opcode",&word,line);
            setBits(1,"destin",&word,line);
            /*make word for the register*/
            setBits(rval,"addres",&regWord,line);            
            addBinWord(binWordHead,word);
            addBinWord(binWordHead,regWord); 
        }
        *ic+=2;/*1 for PSW and 1 for the register*/
        return 0;   
    }else if(rval == ERROR)
    {
        return ERROR;
    }
    rval=isDirectiveAddressing(buff,line);
    if(rval==VALID)
    {
        if(secondRun==ON)
        {
            binWord labelWord;
            int address;            
            initWord(&labelWord);
            if((address=isInTheList(buff,labelsHead,line))==ERROR)
                return ERROR;
            
            setBits(functionIndex,"opcode",&word,line);
            setBits(3,"destin",&word,line);
            /*make word for the label*/
            setBits(address,"addres",&labelWord,line);
            setBits(2,"era",&labelWord,line);                        
            addBinWord(binWordHead,word);
            addBinWord(binWordHead,labelWord); 
        }
        *ic+=2;
        return 0;   
    }else if(rval == ERROR)
    {
        return ERROR;
    }
    rval=isMatrix(buff,line,&r1,&r2);/*NULL becuse we donte need the regestres*/
    if(rval==VALID)
    {
        if(secondRun==ON)
        {
            binWord labelMetrix,regMetrix;           
            int address;
            initWord(&labelMetrix);
            initWord(&regMetrix);
 
            if((address=isInTheList(buff,labelsHead,line))==ERROR)
                return ERROR;
            
            setBits(functionIndex,"opcode",&word,line);
            setBits(2,"destin",&word,line);
            /*make word for the label*/
            setBits(address,"addres",&labelMetrix,line);
            setBits(2,"era",&labelMetrix,line);
            /*make word for the registers of the metrix*/
            setBits(r1,"orginReg",&regMetrix,line);
            setBits(r2,"destReg",&regMetrix,line);

            addBinWord(binWordHead,word);
            addBinWord(binWordHead,labelMetrix); 
            addBinWord(binWordHead,regMetrix);
        }
        *ic+=3; /*1 for PSW and 2 for the metrix*/
        return 0;   
    }else if(rval == ERROR)
    {
        return ERROR;
    } 
     printf ("ERROR:line %d:implicit declaration of dest oprend in function ‘%s’ \n",line,_action[functionIndex].name);
    return ERROR;
} else if(_action[functionIndex].operends==2)
    {
        int regDirectOrgin,regDirectDest;
        char *tok;
        binWordList *binWordBuff;
        binWordBuff=NULL;
        if(cammaCounter!=1)
        {
            if(cammaCounter==0)
            {
                printf ("ERROR:line %d:no , between the operends \n",line);
                            return ERROR;
            }else if(cammaCounter>0)
            {
                printf ("ERROR:line %d:implicit declaration of the oprends in function ‘%s’ \n",line,_action[functionIndex].name);
                return ERROR;
            }    
        }
        if(secondRun==ON)
        {
            setBits(functionIndex,"opcode",&word,line);                
        }        
        firstOp=EMPTY;
        secondOp=EMPTY;
        /*calculet the first operend*/
        tok=strtok(buff,",");
        if(!isEmpty(tok)||*buff==',')
        {
            printf("ERROR:line %d: miseeng a first operend \n",line);
            return ERROR;
        }
        rval=isImmediateAddressing(tok,line);
        if(rval == ERROR)
        {
            return ERROR;
        }else if(rval==VALID)
        {
            firstOp=IMMEDIATELY;
            if(secondRun==ON)
            {
                int intData;
                char asterisk='#';
                binWord data;
                initWord(&data);
                setBits(0,"orgin",&word,line);
                
                while(*tok!=asterisk){tok++;}
                tok++;/*skip the asterisk */
                intData=strtol(tok,&tok,10);
                
                  /*decod the second word*/                
                  if(setBits(intData,"immediate",&data,line)==ERROR)
                        return ERROR;
                  addBinWord(&binWordBuff,data);     
    
            }  
            
        }
        if(firstOp==EMPTY)
        {
            
            if(secondRun==ON)
            {
                rval=isMatrix(tok,line,&r1,&r2);
            }else
            {
                rval=isMatrix(tok,line,NULL,NULL);/*NULL becuse we donte need the regestres*/                
            }
            if(rval == ERROR){return ERROR;}
            else if(rval==VALID)
            {   
                if(secondRun==ON)
                {
                    binWord labelMetrix,regMetrix;           
                    int address;
                    initWord(&labelMetrix);
                    initWord(&regMetrix);
         
                    if((address=isInTheList(buff,labelsHead,line))==ERROR)
                        return ERROR;
                        
                    setBits(2,"orgin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelMetrix,line);
                    setBits(2,"era",&labelMetrix,line);
                    /*make word for the registers of the metrix*/
                    setBits(r1,"orginReg",&regMetrix,line);
                    setBits(r2,"destReg",&regMetrix,line);
                    addBinWord(&binWordBuff,labelMetrix); 
                    addBinWord(&binWordBuff,regMetrix);
                }
                firstOp=MATRIX;
            }
        }
       if(firstOp==EMPTY)
        {
            rval=isRegister(tok);
            if(rval!=NOT_EXIST)
            {
                if(secondRun==ON)
                {
                    /*decode the register !!*/
                    binWord regWord;
                    initWord(&regWord);
                    setBits(1,"orgin",&word,line);
                    /*make word for the register*/
                    setBits(rval,"addres",&regWord,line);            
                    addBinWord(&binWordBuff,regWord);
                    regDirectOrgin=rval;
                    
                }
                firstOp=REG_DIRECT;
            }
        }
        if(firstOp==EMPTY)
        {
            rval=isDirectiveAddressing(tok,line);
            if(rval == ERROR){return ERROR;}
            else if(rval==VALID)
            {
                if(secondRun==ON)
                {
                    binWord labelWord;
                    int address;
                    initWord(&labelWord);
                    if((address=isInTheList(buff,labelsHead,line))==ERROR)
                        return ERROR;
                    /*decod the orgin*/
                    setBits(3,"orgin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelWord,line);
                    setBits(2,"era",&labelWord,line);                        
                    addBinWord(&binWordBuff,labelWord); 
                }
                firstOp=DIRECT;
            }
        }
        if(firstOp==EMPTY)
        {
            printf("ERROR:line %d: unvalid addressing method \n",line);
            return ERROR;
        }

        /*calculet the second operend*/
         tok=strtok(NULL,"\0");
         if((!isEmpty(tok)))
        {
            printf("ERROR:line %d: miseeng a second operend \n",line);
            return ERROR;
        }
        rval=isImmediateAddressing(tok,line);
        if(rval == ERROR)
        {
            return ERROR;
        }else if(rval==VALID)
        {
            secondOp=IMMEDIATELY;
            if(secondRun==ON)
            {
                int intData;
                char asterisk='#';
                binWord data;
                initWord(&data);
                setBits(0,"orgin",&word,line);
                
                while(*tok!=asterisk){tok++;}
                tok++;/*skip the asterisk */
                intData=strtol(tok,&tok,10);
                
                  /*decod the second word*/                
                  if(setBits(intData,"immediate",&data,line)==ERROR)
                        return ERROR;
                  addBinWord(&binWordBuff,data);     
    
            }  
        }

        if(secondOp==EMPTY)
        {
            if(secondRun==ON)
            {
                rval=isMatrix(tok,line,&r1,&r2);
            }else
            {
                rval=isMatrix(tok,line,NULL,NULL);/*NULL becuse we donte need the regestres*/                
            }
            if(rval == ERROR){return ERROR;}
            else if(rval==VALID)
            {
                if(secondRun==ON)
                {
                    binWord labelMetrix,regMetrix;           
                    int address;
                    initWord(&labelMetrix);
                    initWord(&regMetrix);
         
                    if((address=isInTheList(buff,labelsHead,line))==ERROR)
                        return ERROR;
                        
                    setBits(2,"destin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelMetrix,line);
                    setBits(2,"era",&labelMetrix,line);
                    /*make word for the registers of the metrix*/
                    setBits(r1,"orginReg",&regMetrix,line);
                    setBits(r2,"destReg",&regMetrix,line);
                    addBinWord(&binWordBuff,labelMetrix); 
                    addBinWord(&binWordBuff,regMetrix);
                }
                secondOp=MATRIX;
            }
        }
       if(secondOp==EMPTY)
        {
            rval=isRegister(tok);
            if(rval!=NOT_EXIST)
            {
                if(secondRun==ON)
                {   binWord regWord;                    
                    regDirectDest=rval;
                    /*decode the register !!*/
                    initWord(&regWord);                    
                    setBits(1,"destin",&word,line);
                    /*make word for the register*/
                    setBits(rval,"addres",&regWord,line);            
                    addBinWord(&binWordBuff,regWord); 
                }
                secondOp=REG_DIRECT;
            }
        }
        if(secondOp==EMPTY)
        {
            rval=isDirectiveAddressing(tok,line);
            if(rval == ERROR){return ERROR;}
            else if(rval==VALID)
            {
                if(secondRun==ON)
                {
                    binWord labelWord;
                    int address;
                    initWord(&labelWord);
                    if((address=isInTheList(buff,labelsHead,line))==ERROR)
                        return ERROR;
                    /*decod the destin*/
                    setBits(3,"destin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelWord,line);
                    setBits(2,"era",&labelWord,line);                        
                    addBinWord(&binWordBuff,labelWord); 
                }
                secondOp=DIRECT;
            }
        }
        if(secondOp==EMPTY)
        {
            printf("ERROR:line %d: unvalid addressing method ",line);
            return ERROR;
        }
    
    /*test if the  oprends is a ligals oprends*/
    if(functionIndex==4)/*4 == lea in the cmd[]arry*/
    {
        if(firstOp!=DIRECT&&firstOp!=MATRIX)
        {
        printf("ERROR:line %d:lea can get in the first operend only DIRECT OR MATRIX METHOD !!",line);
        return ERROR;
        }
        if(secondOp==IMMEDIATELY)
        {
        printf("ERROR:line %d:lea can't get in the second operend a IMMEDIATELY METHOD !!",line);
        return ERROR;
        }
    }

    if(secondOp==IMMEDIATELY&&functionIndex!=12&&functionIndex!=VALID)/*the function is not cmp or prn*/
    {
        printf("ERROR:line %d:only cmp or prn can get in the second operend in a IMMEDIATELY METHOD !!",line);
        return ERROR;
    }

    /*if we got here the oprends is ligal*/
   
        /*now calculate the "ic cost"*/
        switch(firstOp)
        {
            case IMMEDIATELY:
            case DIRECT:
            case REG_DIRECT:
            {
                firstCost =1;
                break;
            }
            case MATRIX:
            {
                firstCost=2;
                break;  
            }
            case EMPTY:
            {
                return ERROR;
            }
        }
        switch(secondOp)
        {
            case IMMEDIATELY:
            case DIRECT:
            case REG_DIRECT:
            {
                secondCost =1;
                break;
            }
            case MATRIX:
            {
                secondCost=2;
                break;  
            }
            case EMPTY:
            {
                return ERROR;
            }
        }

        if(firstOp==REG_DIRECT&&secondOp==REG_DIRECT)
        {
            if(secondRun==ON)
            {
                binWordList* temp;                
                binWord registers;
                initWord(&registers);
                setBits(regDirectOrgin,"orginReg",&registers,line);
                setBits(regDirectDest,"destReg",&registers,line);
                addBinWord(binWordHead,word);
                /*delete the buf binWord */
                while (binWordBuff != NULL)
                { 
                    temp = binWordBuff; 
                    temp ->next=NULL;
                    binWordBuff = binWordBuff->next;
                    free(temp);
                } 
            }
            *ic+=2;/*1 for psw and 1 fo the common word for the registers*/
            return 0;
        }

        if(secondRun==ON)
        {
            addBinWord(binWordHead,word);
            catBinWordList(binWordHead,&binWordBuff);
        }    
        *ic+=(secondCost+firstCost + 1);/*1 for psw*/
        return 0;

    }
    return ERROR;
}

 int isInTheList(char *label,labelsList **labelsHead,int line)
 {
    SWITCHER isIn=OFF;
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
        isIn=ON;
        }
      }
      labelsHead = &( (*labelsHead)->next);    
    }
    if(isIn==OFF)
    {
        printf("error:line %d:the label in the function not declir enywear !!\n",line);
            return ERROR;
    }
    return 0;
}
void initWord(binWord *word)
{
    word->opcode=word->orgin=word->dest=word->era=0;      
}