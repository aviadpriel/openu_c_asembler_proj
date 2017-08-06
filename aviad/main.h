#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ERROR -1
#define LABEL_MAX_LEN 30
#define WORD_SIZE 10
typedef enum {ON,OFF,WAIT} SWITCHER;

typedef struct dataList{
int data:WORD_SIZE;
struct dataList *next;
}dataList;

typedef struct labelsList {
  char *label;
SWITCHER action;
SWITCHER external;
int address;
struct labelsList *ls, *rs;
} labelsList;

void insertData(dataList **head,int data);
int insertLabel(labelsList **root, char *label,SWITCHER action,SWITCHER external,
  int address);