#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WORD_SIZE 10
typedef enum {ON,OFF,WAIT} SWITCHER;
typedef struct labelsTree {
  char *label;
SWITCHER action;
SWITCHER external;
int address;
struct labelsTree *ls, *rs;
} labelsTree;
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


/************labelsTree functions*******/
labelsTree* newLabel(char *label,int address,SWITCHER action,SWITCHER external)
{
  labelsTree *p = (labelsTree*)malloc(sizeof(labelsTree));
  if (!p) {
    printf("memmory error exit\n");
    exit(1);
  }
  p->label = label;
  p->action=action;
  p->external=external;
  p->address=address;
  p->ls = p->rs = NULL;
  return p;
}
/*if */
int insertLabel(labelsTree **root, char *label,SWITCHER action,SWITCHER external,
  int address)
{
/* insert using a loop and pointer to pointer*/
  while(*root)
  {
     if(strcmp(label,(*root)->label)<0)
       root = &( (*root)->ls);
     else if(strcmp(label,(*root)->label)>0)
       root = &( (*root)->rs);
     else/*error the label is in the list */
    {
      printf("the label %s is in the list \n",label);
      return 0;
    }
  }
  *root = newLabel(label,address,action,external);
  return 1;
}
