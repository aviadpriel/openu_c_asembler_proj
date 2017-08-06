#include "main.h"
int matF(char *command,char * label,SWITCHER lableFlag,int *dc,dataList **dataHead
    ,labelsList **labelsHead);
int isRegister(char *buf);
int main()
{
dataList *dataHead =NULL;
labelsList *labelsHead=NULL; 
int dc =0;   
dc=0;
char c[]={" [2][3] 0,1,2,3   \0"}; 

printf("%d ! \n",matF(c,"aviad",ON,&dc,&dataHead,&labelsHead));
return 0;
}
