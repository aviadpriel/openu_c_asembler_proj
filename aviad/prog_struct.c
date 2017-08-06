#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WORD_SIZE 10
#define ERROR -1

typedef enum {ON,OFF,WAIT} SWITCHER;
typedef struct labelsList {
  char *label;
SWITCHER action;
SWITCHER external;
int address;
struct labelsList *next;
} labelsList;
typedef struct dataList{
int data:WORD_SIZE;
struct dataList *next;
}dataList;
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


void insertData(dataList **head,int data)
{
   dataList *current=(*head);
  if(!(*head))
  {
  (*head)=newData(data);

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
labelsList* newLabel(char *label,int address,SWITCHER action,SWITCHER external)
{
  labelsList *p = (labelsList*)malloc(sizeof(labelsList));
  if (!p) {
    printf("memmory error exit\n");
    exit(1);
  }
  p->label = label;
  p->action=action;
  p->external=external;
  p->address=address;
  p->next =NULL;
  return p;
}
/*if */
int addLabel(labelsList **root, char *label,SWITCHER action,SWITCHER external,
  int address)
{
/* insert using a loop and pointer to pointer*/
  while(*root)
  {
     if(strcmp(label,(*root)->label)!=0)
       root = &( (*root)->next);
     else/*error the label is in the list */
    {
      printf("the label %s is in the list \n",label);
      return ERROR;
    }
  }
  *root = newLabel(label,address,action,external);
  return 1;
}
