#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#define ERROR -1
#define LINE_LENGTH 81
typedef enum {ON,OFF,WHIT} SWITCHER;
int isDirective(char *command);
int isAction(char *command);
int isRegister(char *buf);
char * isLabel(char * buf,int *error);
/*evgeny : Declaration of functions that must be written (in another file)*/

/*
for active functions
return 0 if ok
return ERROR if have any error
*/
int firstOperend(char * buf,int functionIndex,int *ic);
/*
for active functions
work from inside firstOperend function
return 0 if ok
return ERROR if have any error
*/
int secondOperend(char * buf,int functionIndex,int *addressingMethod);
/*pasdocode for first_run(char []line);*/

int first_run(FILE *fp,int *dc,int *ic)
{

  SWITCHER errorFlag = OFF; /*errorFlag check if we can to go to second_run */
  char *line;
  int lineCounter=1 ;/*for print errors*/
  /*Confirming the cursor to the beginning of the file*/
  fseek(fp,0,SEEK_SET);

  while(fgets( line, LINE_LENGTH,fp))
  {

    int error =0,c,functionIndex;
    char *buff;
    char *label;
	buff=strtok(line," ");
	label=isLabel(buff,&error);
    if(error ==ERROR)
    {
      errorFlag=ON;
    }else if(label!=NULL)/*we have a label!! :)*/
    {
      buff= strtok(NULL," ");/*we go to the next word*/
    }
    if((functionIndex =isDirective(buff))!=-2)/*the function is directive stetment*/
    {
      buff= strtok(NULL,"\n");/*we get the rest of the line*/

    } else if((functionIndex =isAction(buff))!=-2)/*the function is action stetment*/
    {
      buff= strtok(NULL,"\n");/*we get the rest of the line*/
	  if(firstOperend(buff,functionIndex,&ic)==ERROR)
	  {
		      errorFlag=ON;
	  }
    }
    else /*unligal stetment print error*/
    {
      printf("evgeny put a normal error messege %s")
    }

    lineCounter++;
  }/*end of big while*/

  if(errorFlag==OFF)
  {
	return 0;  
  }
  else
  return ERROR;
}
