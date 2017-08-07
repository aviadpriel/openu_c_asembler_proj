
#define WORD_SIZE 10
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
void insertData(dataList **head,unsigned data);
labelsList* newLabel(char *label,int address,SWITCHER action,SWITCHER external);
int addLabel(labelsList **labelsHead,char *label,SWITCHER action,SWITCHER external,
SWITCHER data,int address,int line);
