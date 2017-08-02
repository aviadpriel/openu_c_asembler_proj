#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DATA 0
#define EXTERN 1
#define CODE 2

typedef struct symbol_tree symbol_tree;
#define START_LINE 100 /*defualt start*/
#define LINE_LEN 81 /*defualt line */
#define SYMBOLE_MAX_LEN 30
int issymbol(char *p);
int insert(symbol_tree **root, char *symbol,int type);
void group1(char *command);
void group2(char *command);
void group3(char *command);
void group4(char *command);
void group5(char *command);
void error(char *command,int commandIndex);

typedef struct commands
    {
    char *name;
    void(*func)(char *command,int commandIndex);
  }commands ;commands cmd[]={
           /*{".data",group1},
           {".string",group1},
           {".mat",group1},
           {".extern",group2},
           {".entry",group2},
           {"mov",group3},
           {"cmp",group3},
           {"add",group3},
           {"sub",group3},
           {"lea",group3},
           {"not",group4},
           {"clr",group4},
           {"inc",group4},
           {"dec",group4},
           {"jmp",group4},
           {"bne",group4},
           {"red",group4},
           {"prn",group4},
           {"jsr",group4},
           {"rts",group4},
           {"stop",group4},*/
           {"error",error}};typedef struct commands
    {
    char *name;
    void(*func)(char *command,int commandIndex);
  }commands ;commands cmd[]={
           /*{".data",group1},
           {".string",group1},
           {".mat",group1},
           {".extern",group2},
           {".entry",group2},
           {"mov",group3},
           {"cmp",group3},
           {"add",group3},
           {"sub",group3},
           {"lea",group3},
           {"not",group4},
           {"clr",group4},
           {"inc",group4},
           {"dec",group4},
           {"jmp",group4},
           {"bne",group4},
           {"red",group4},
           {"prn",group4},
           {"jsr",group4},
           {"rts",group4},
           {"stop",group4},*/
           {"error",error}};
struct label_tree {
  char *label;
  int action;
  int type;
  struct symbol_tree *ls, *rs;
};
enum SYMBOL{TRUTH,FALSE};
