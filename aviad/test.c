#include<stdio.h>
#include<ctype.h>
#include<string.h>
int issymbol(char *p,int line);
int main(void)
{
    int i=0;
    char a[]="5aviad priel";
    char *p=strtok(a," "),*tmp;
    
    printf("the anser is : %d",issymbol(p));
    return 0;
}
/*
char* p - the word we wont to cheke
return -1 if is unligal word ,1 if is symbol,0if is not
*/
int issymbol(char *p)
{
    int i=0;
    if(!(isalpha(p[i])))
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