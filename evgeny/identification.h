

#ifndef EXTENTLIST
#define EXTENTLIST
typedef struct extEntList{
  char *label;
  int address;
  DIRECRIVE_FUNCTION type;
  struct extEntList *next;
}extEntList;

#endif
void initWord(binWord *word);

int countrChars(char *word, char c,int line);

int setBits(int data,char * type,binWord * word,int line);

void addBinWord(binWordList **binWordHead,binWord *word,int address);

int isImmediateAddressing(char * buff,int line);

int isRegister(char *buf);
int isDirectiveAddressing(char * buff,int line);

int isInTheList(char *label,labelsList **labelsHead,int line,int *externalFlag,extEntList **extEntHead,int address);

int isMatrix(char *buff,int line,int * r1,int *r2);

int isEmpty(char * buff);

void catBinWordList(binWordList **binWordHead,binWordList **binWordBuff,int curIC);

void catExtEntList(extEntList **extEntHead,extEntList **extEntBuff,int ic);

void binWordToStrangeBase(binWord *addressWord,binWord *dataWord);

void addExtEnt(extEntList **extEntHead,char *label,int address,DIRECRIVE_FUNCTION type);

