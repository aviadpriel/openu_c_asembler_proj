#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
typedef enum {aviad,shaked} priel;
void foa(int *a);
typedef struct foo{
priel a;
int num;
}foo;
int main(void)
{
int a=5;
foa(&a);
printf("%d\n",a);
return 0;
}

void foa(int *a)
{
int b=*a;
b++;
*a=b;
}
