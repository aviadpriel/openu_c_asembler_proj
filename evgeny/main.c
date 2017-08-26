#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include "first_run.h"
#include "second_run.h"*/
#include "consts.h"
#include "struct.h"

#define LINE_ARG_SUFFIX 4
int first_run(FILE *fp,labelsList **labelsHead,dataList **dataHead,int *dc,int *ic);
int second_run(FILE *fp,labelsList **labelsHead,dataList **dataHead, char *file_name,int DC, int IC);

/*adds suffix ".as" to command line argument name*/
void modify_name(char **file_name, int size, char *argv);
void freeList(labelsList *l_head);
/*void freeData(dataList *d_head);*/

/*****************************************************************************************************************************************************
	*Function: int main(...)
	*Input: file_name.as (1 to n files)
	*Return: 0 on success
	*Description: read assembly files and decode them to machine language, Outputs 3 organized file_name.ob/ent/ext in a "strange 4-base" display. 
*****************************************************************************************************************************************************/
int main(int argc, char *argv[])
{
    int i, IC, DC;
    char *file_name = NULL;/*pointer to current argument file name*/
    FILE *fp = NULL;
    labelsList *l_head = NULL;
    dataList *d_head = NULL;
    IC = DC = 0;

    if (argc == 1)
    {
        printf("No arguments were given");
        exit(1);
    }
    /*every new loop run is a different file.as*/
    for(i = 1; i < argc; i++)
    {
        l_head = NULL;
        d_head = NULL;
        modify_name(&file_name,(strlen(argv[i]) + LINE_ARG_SUFFIX),argv[i]);
        fp = fopen(file_name, "r");
        if(!fp)
            {
                printf("file opening failed\n");
                exit(1);
            }
        printf("compiling %s file\n", file_name);
        if(first_run(fp, &l_head, &d_head, &DC, &IC))
        {
            printf("file \"%s\" compile failed.\n continue to next file.\n", file_name);
            continue;
        }
        second_run(fp, &l_head, &d_head,argv[i], DC, IC);
        fclose(fp);
        freeList(l_head);
        /*freeData(d_head);*/
        free(file_name);
    }/*end of for*/
    printf("assembly is finished.\nValid files has been processed successfully.\n");
    return 0;
}

/*****************************************************************************************************************************************************
	*Function: modify_name(...)
    *Input: char *file_name- pointer to modified file name to be.
            int size- sizeof memmory allocation requirement for modified file name.
            char *argv- the original name to be modified. 
	*return: none
	*Description: gets the command line argument file name and adds the suffix ".as"
*****************************************************************************************************************************************************/
void modify_name(char **file_name, int size, char *argv)
{
    *file_name = (char*)malloc(sizeof(char) * size);
        if(!file_name)
            {
                printf("memmory allocation failed\n");
                return;
            }
    strcpy(*file_name, argv);
    strcat(*file_name,".as\0");
}

void freeList(labelsList *l_head)
{
    labelsList *temp;
    while(l_head)
    {
        temp = l_head->next;
        free(l_head);
        l_head = temp;
    }
}

/*void freeData(dataList *d_head)
{
    dataList *temp;
    while(d_head)
    {
        temp = d_head->next;
        free(d_head);
        d_head = temp;
    }
}*/