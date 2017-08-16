
void initWord(binWord *word);

int countrChars(char *word, char c,int line);

int setBits(int data,char * type,binWord * word,int line);

void addBinWord(binWordList **binWordHead,binWord *word,int address);

int isImmediateAddressing(char * buff,int line);

int isRegister(char *buf);
int isDirectiveAddressing(char * buff,int line);

int isInTheList(char *label,labelsList **labelsHead,int line,int *externalFlag);

int isMatrix(char *buff,int line,int * r1,int *r2);

int isEmpty(char * buff);

void catBinWordList(binWordList **binWordHead,binWordList **binWordBuff,int curIC);

void binWordToStrangeBase(binWord *addressWord,binWord *dataWord);