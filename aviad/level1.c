#include "headerfile.h"
int main(int argc, char const *argv[]) {
int dc=0,ic=100;
FILE  *fp;
symbol_tree *root=NULL;
int symbolFlag;
char line[LINE_LEN],*fild;
fp=fopen(argv[1],"r");
if (!fp){
 exit(1);
}

while (!(fgets(line,LINE_LEN,fp)))
{
int i;
int flag;
char symbol[SYMBOLE_MAX_LEN];
fild=strtok(line," ");
if((flag=issymbol(fild))==1)/*is symbol*/
{
  strncpy(symbol,fild,SYMBOLE_MAX_LEN);
  symbolFlag=TRUTH;
  fild=strtok(NULL," ");
}
else if (flag==-1) {
  /* code */
}
for(i=0;cmd[i].func!=error;i++)
          {
              if(strcmp(fild,cmd[i].name)==0)
                  break;
          }
              if(cmd[i].func==NULL)
              {
                  fprintf(stderr,"error:unligal commend ; %s\n",fild);
              }
              else
                {
                  if(symbolFlag==TRUTH)
                  {
                    if (i<3)
                    {
                      flag=insert(&root,symbol,DATA);
                    }
                    else if(i==4)
                    {
                      printf("warning:trying put a symbol before .entery function.\n");
                    }
                    else if(i==5) /*.entery wharnnig*/
                    {
                      printf("warning:trying put a symbol before .entery function.\n");
                    }
                    else if (i>5 && cmd[i].func!=error)
                    {
                    insert(&root,symbol,CODE);
                    }
                  }

                  if(cmd[i].func==error)
                  {
                      printf("error: the commend %s is not found\n",fild );
                  }
                  else/*the commend is fine*/
                  {
                    fild = strtok(NULL,"\n");
                    (*(cmd[i].func))(fild,i);
                  }
              }
}

return 0;
}



symbol_tree* newsymbol(char *symbol,int type) {
  symbol_tree *p = (symbol_tree*)malloc(sizeof(symbol_tree));
  p->symbol = symbol;
  p->type=type;
  p->ls = p->rs = NULL;
  return p;
}

int insert(symbol_tree **root, char *symbol,int type) {
/* insert using a loop and pointer to pointer*/
  while(*root)
  {
     if(strcmp(symbol,(*root)->symbol)<0)
       root = &( (*root)->ls);
     else if(strcmp(symbol,(*root)->symbol)>0)
       root = &( (*root)->rs);
     else/*error the symbol is in the list */
    {
      printf("the symbol %s is in the list in line \n",symbol);
      return 0;
    }
  }
  *root = newsymbol(symbol,type);
  return 1;
}
char * issymbol(char *p,int *error)
{
  int i=0;
  if(!(isalpha(p[i])&&p[i]!='.'))
  {
  printf("unligal word : %s \n",p);
  *error=0;
  return NULL;
  }
  while(p[i]!='\0')
  {
      if(p[i]==':')
      {
          i++;
          if(p[i]=='\0')
          {
          i--;
          p[i]='\0';
          *error=1;
          return p;/*is symbol!!!*/
        }
          else
          {
          printf("unligal word : %s \n",p);
          *error=0;
          return NULL;
          }
      }
      else
      i++;
  }
*error=1; /*no error*/
return NULL;/*but is not a symbol*/
}
