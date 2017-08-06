#include "main.h"
int matF(char *command,char * label,SWITCHER lableFlag,int *dc,dataList **head
    ,labelsTree **root);
int isRegister(char *buf);
int main()
{
dataList *head =NULL;
labelsTree *root=NULL; 
int dc =0;   
dc=0;
char c[]={" [2][3] 0,1,2,3   \0"}; 

printf("%d ! \n",matF(c,"aviad",ON,&dc,&head,&root));
return 0;
}
