#include "consts.h"
#include "struct.h"
#include "identification.h"
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>

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

/*pasdo-code for crazy decodeAction
*1.idenfy the addressing methds
*2.idenfy cost
*3.idenfy data and find addres of labels
*4 if have errors stop
*5 decod the words in binWordList;
*6 learn to infi 2
*/

int identification(char * buff,int functionIndex,int line,int *ic,SWITCHER secondRun,binWordList **binWordHead,labelsList **labelsHead,extEntList **extEntHead)
{    
binWord word;
addressing_method firstOp,secondOp;    
int firstCost=0,secondCost=0,rval=0,r1=0,r2=0,externalFlag=0;
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
    addBinWord(binWordHead,&word,*ic);
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

                addBinWord(binWordHead,&word,*ic);
                addBinWord(binWordHead,&data,(*ic)+1);                  
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
            initWord(&regWord);            
            setBits(functionIndex,"opcode",&word,line);
            setBits(3,"destin",&word,line);
            /*make word for the register*/
            setBits(rval,"opcode",&regWord,line);            
            addBinWord(binWordHead,&word,*ic);
            addBinWord(binWordHead,&regWord,(*ic)+1); 
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
            if((address=isInTheList(buff,labelsHead,line,&externalFlag,extEntHead,(*ic)+1))==ERROR)
                return ERROR;
            
            setBits(functionIndex,"opcode",&word,line);
            setBits(1,"destin",&word,line);
            /*make word for the label*/
            setBits(address,"addres",&labelWord,line);
            if(externalFlag)
            {
                setBits(1,"era",&labelWord,line);                
            }
            else
            {
                setBits(2,"era",&labelWord,line);
            }                        
            addBinWord(binWordHead,&word,*ic);
            addBinWord(binWordHead,&labelWord,(*ic)+1); 
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
 
            if((address=isInTheList(buff,labelsHead,line,&externalFlag,extEntHead,(*ic)+1))==ERROR)
                return ERROR;
            
            setBits(functionIndex,"opcode",&word,line);
            setBits(2,"destin",&word,line);
            /*make word for the label*/
            setBits(address,"addres",&labelMetrix,line);
            if(externalFlag)
            {
                setBits(1,"era",&labelMetrix,line);                
            }
            else
            {
                setBits(2,"era",&labelMetrix,line);
            }     
            /*make word for the registers of the metrix*/
            setBits(r1,"orginReg",&regMetrix,line);
            setBits(r2,"destReg",&regMetrix,line);

            addBinWord(binWordHead,&word,*ic);
            addBinWord(binWordHead,&labelMetrix,(*ic)+1); 
            addBinWord(binWordHead,&regMetrix,(*ic)+2);
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
        extEntList *extEntBuff;
        int binWordCunter =0;
        binWordBuff=NULL;
        tok=NULL;
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
                  addBinWord(&binWordBuff,&data,0);
                  addExtEnt(&extEntBuff,NULL,ERROR,DATA);                  
                  binWordCunter++;
    
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
         
                    if((address=isInTheList(buff,labelsHead,line,&externalFlag,&extEntBuff,binWordCunter))==ERROR)
                        return ERROR;
                        
                    setBits(2,"orgin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelMetrix,line);
                    if(externalFlag)
                    {
                        setBits(1,"era",&labelMetrix,line);
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);                        
                    }
                    else
                    {
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);                        
                        setBits(2,"era",&labelMetrix,line);
                    }     
                    /*make word for the registers of the metrix*/
                    setBits(r1,"orginReg",&regMetrix,line);
                    setBits(r2,"destReg",&regMetrix,line);
                    addBinWord(&binWordBuff,&labelMetrix,0); 
                    addBinWord(&binWordBuff,&regMetrix,0);
                    binWordCunter+=2;
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
                    setBits(3,"orgin",&word,line);
                    /*make word for the register*/
                    setBits(rval,"opcode",&regWord,line);            
                    addBinWord(&binWordBuff,&regWord,0);
                    addExtEnt(&extEntBuff,NULL,ERROR,DATA);                    
                    binWordCunter++;
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
                    if((address=isInTheList(buff,labelsHead,line,&externalFlag,&extEntBuff,binWordCunter))==ERROR)
                    return ERROR;
                    /*decod the orgin*/
                    setBits(3,"orgin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelWord,line);
                    if(externalFlag)
                    {
                        setBits(1,"era",&labelWord,line);                
                    }
                    else
                    {
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);                        
                        setBits(2,"era",&labelWord,line);
                    }     
                    addBinWord(&binWordBuff,&labelWord,0);
                    binWordCunter++; 
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
                  addBinWord(&binWordBuff,&data,0);
                  addExtEnt(&extEntBuff,NULL,ERROR,DATA);
                  
                  binWordCunter++;     
    
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
         
                    if((address=isInTheList(tok,labelsHead,line,&externalFlag,&extEntBuff,binWordCunter))==ERROR)
                    return ERROR;
                        
                    setBits(2,"destin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelMetrix,line);
                    if(externalFlag)
                    {
                        setBits(1,"era",&labelMetrix,line); 
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);
                        
                    }
                    else
                    {
                        setBits(2,"era",&labelMetrix,line);
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);
                        
                    }     
                    /*make word for the registers of the metrix*/
                    setBits(r1,"orginReg",&regMetrix,line);
                    setBits(r2,"destReg",&regMetrix,line);
                    addBinWord(&binWordBuff,&labelMetrix,0); 
                    addBinWord(&binWordBuff,&regMetrix,0);
                    binWordCunter+=2;
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
                    setBits(3,"destin",&word,line);
                    /*make word for the register*/
                    setBits(rval,"addres",&regWord,line);            
                    addBinWord(&binWordBuff,&regWord,0);
                    addExtEnt(&extEntBuff,NULL,ERROR,DATA);
                    
                    binWordCunter++;
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
                    if((address=isInTheList(tok,labelsHead,line,&externalFlag,&extEntBuff,binWordCunter))==ERROR)
                    return ERROR;
                    /*decod the destin*/
                    setBits(1,"destin",&word,line);
                    /*make word for the label*/
                    setBits(address,"addres",&labelWord,line);
                    if(externalFlag)
                    {
                        setBits(1,"era",&labelWord,line);                
                    }
                    else
                    {
                        addExtEnt(&extEntBuff,NULL,ERROR,DATA);                        
                        setBits(2,"era",&labelWord,line);
                    }     
                    addBinWord(&binWordBuff,&labelWord,0);
                    binWordCunter++; 
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
                addBinWord(binWordHead,&word,*ic);
                addBinWord(binWordHead,&registers,(*ic)+1);
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
            addBinWord(binWordHead,&word,(*ic));/*spsw*/
            catBinWordList(binWordHead,&binWordBuff,*ic);
            catExtEntList(extEntHead,&extEntBuff,*ic);
        }    
        *ic+=(secondCost+firstCost + 1);/*1 for psw*/
        return 0;

    }
    return ERROR;
}

/*
מחברת 2 מבני נתונים מסוג 
binWordList
*/
void catBinWordList(binWordList **binWordHead,binWordList **binWordBuff,int curIC)
{
  int add=curIC+1; 
  while(*binWordHead)
  {
    binWordHead = &( (*binWordHead)->next);
  }
  *binWordHead=*binWordBuff;
  while(*binWordHead)
  {
    (*binWordHead)->address=add;
    binWordHead = &( (*binWordHead)->next);
    add++;    
  }  
}
