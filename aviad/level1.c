#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define START_LINE 100 /*defualt start*/
#define LINE_LEN 80 /*defualt line */
int issymbol(char *p);
typedef struct command
      {
      char *name;
      void(*func)(char *command);
      }cmd[]={
             {"read_comp",read_comp},
             {"print_comp",print_comp},
             {"add_comp",add_comp},
             {"sub_comp",sub_comp},
             {"mult_comp_real",mult_comp_real},
             {"mult_comp_img",mult_comp_img},
             {"mult_comp_comp",mult_comp_comp},
             {"abs_comp",abs_comp},
       {"halt\n",halt},
       {"error",NULL}};
typedef struct symbol_tree {
    char *symbol;
    int address;
    struct symbol_tree *ls, *rs;
} symbol_tree;
enum SYMBOL{TRUTH,FALSE};

int main(int argc, char const *argv[]) {
  FILE  *fp;
  char *dataStorage[]={".data",".string",".mat"};
  char line[LINE_LEN],*fild;
  int address ,symbolFlag;
  address =START_LINE;
fp=fopen(argv[1],"r");
if (!fp) {
  printf("cannot open the file : %s\n",argv[1]);
  exit(1);
}

while (!(fgets(line,LINE_LEN,fp)))
{
  int i;
  int flag;
  fild=strtok(line,"");
  if((flag=issymbol(fild))==1)/*is symbol*/
  {
    symbolFlag=TRUTH;
  }
  else if (flag==-1) {
    /* code */
  }
  isdataStorage()
}

  return 0;
}
symbol_tree* newsymbol(char *symbol,int address) {
    symbol_tree *p = (symbol_tree*)malloc(sizeof(symbol_tree));
    p->symbol = symbol;
    p->address=address;
    p->ls = p->rs = NULL;
    return p;
}

void insert(symbol_tree **root, char *symbol,int address) {
/* insert using a loop and pointer to pointer*/
    while(*root) {
	if(strcmp(symbol,(*root)->symbol)<0)
	    root = &( (*root)->ls);
	else if(strcmp(symbol,(*root)->symbol)>0)
	    root = &( (*root)->rs);
  else/*error the symbol is in the list */
  {
  printf("the symbol %s is in the list in line %d \n",symbol,(*root)->address);
  }
    }
    *root = newsymbol(symbol,address);
}
int issymbol(char *p)
{
    int i=0;
    if(!(isalpha(p[i])&&p[i]!='.'))
    {
    printf("unligal word : %s \n",p);
    return -1;
    }
    while(p[i]!='\0')
    {
        if(p[i]==':')
        {
            i++;
            if(p[i]=='\0')
            {
                return 1;/*is symbol!!!*/
            }
            else
            {
            printf("unligal word : %s \n",p);
            return -1;
            }
        }
        else
        i++;
    }
return 0;/*is not a symbol*/
}
