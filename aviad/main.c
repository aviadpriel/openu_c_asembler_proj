#include "struct.h"
int first_run(FILE *fp,labelsList **labelsHead,dataList **dataHead,int *dc,int *ic);
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
freeDataList(dataHead);
freeLabelsList(labelsHead);
return 0;
}
