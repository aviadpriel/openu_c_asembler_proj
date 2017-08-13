#define WORD_SIZE 10
#define ERROR -1

#ifndef SWITCHER_1
#define SWITCHER_1
typedef enum {ON,OFF,WAIT} SWITCHER;
#endif
#ifndef LABELS_LIST_1
#define LABELS_LIST_1
typedef struct labelsList {
  char *label;
SWITCHER data;
SWITCHER action;
SWITCHER external;
int address;
struct labelsList *next;
} labelsList;

int addLabel(labelsList **labelsHead, char *label,SWITCHER action,SWITCHER external,
 SWITCHER data, int address,int line);
void freeLabelsList(labelsList *labelsHead);
#endif
#ifndef DATA_LIST_1
#define DATA_LIST_1
typedef struct dataList{
int data:WORD_SIZE;
struct dataList *next;
}dataList;

void insertData(dataList **dataHead,int data);
void freeDataList(dataList *dataHead);
#endif

#ifndef COMMAND_LIST_1
#define COMMAND_LIST_1
typedef struct commandList{
char *name;  
int operends;
int operendGroup;
}commandList;
#endif




