#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#define WORD_SIZE 10
#define ERROR -1

#ifndef SWITCHER_1
#define SWITCHER_1
typedef enum {ON,OFF,WAIT} SWITCHER;
#endif
typedef struct labelsList {
  char *label;
SWITCHER data;
SWITCHER action;
SWITCHER external;
int address;
struct labelsList *next;
} labelsList;
typedef struct dataList{
int data:WORD_SIZE;
struct dataList *next;
}dataList;
typedef struct commandList{
char *name;  
int operends;
int operendGroup;
}commandList;


void insertData(dataList **dataHead,int data);
int addLabel(labelsList **labelsHead, char *label,SWITCHER action,SWITCHER external,
 SWITCHER data, int address,int line);