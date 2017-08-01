#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define WORD_SIZE 10
typedef enum {ON,OFF,WHIT} SWITCHER;
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
void insertData(dataList **head,unsigned data);
labelsTree* newLabel(char *label,int address,SWITCHER action,SWITCHER external);
int insertLabel(labelsTree **root, char *label,SWITCHER action,SWITCHER external,
  int address);
