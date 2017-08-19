/*****************************************************************************************************************************************************
This is an output file.
will out one of 3:
1. stage1 errors output.
2. stage2 errors output.
3. strange 4 base output.
*****************************************************************************************************************************************************/
#include <stdio.h>
#include "general.h"
#include "stage1.h"
#include "stage2.h"
#include "errors.h"
#include "output.h"//optional

#define OB_SUFFIX .ob
#define ENT_SUFFIX .ent
#define EXT_SUFFIX .ext

void output_entry(char *file_name)
{
    char *output_name = NULL;
    modify_output_name(file_name, "ENT_SUFFIX", output_name)
    //creating 4 base output.
    FILE *fp = fopen(output_name,"w");
    free(output_name);
}

void output_extern(char *file_name)
{
    char *output_name = NULL;
    modify_output_name(file_name, "EXT_SUFFIX", output_name)
    //creating 4 base output.
    FILE *fp = fopen(output_name,"w");
    free(output_name);
}

void output_object(char *file_name)
{
    char *output_name = NULL;
    modify_output_name(file_name, "OB_SUFFIX", output_name)
    //creating 4 base output.
    FILE *fp = fopen(output_name,"w");
    free(output_name);
}

/*****************************************************************************************************************************************************
	*Function: char *modify_output_name(...)
    *Input: char *file_name- the current file name to be assembeled.
            char *suffix- the suffix to current file name deppending on file creation type (.ob, .ext, .entry)
            char *output_name pointer which will point to relevant file name to be created.
	*Return: pointer to relevant output file name.
	*Description: it takes the current file name and adds the relevant suffix for a new file to be created (.ob, .ext, .ent).
*****************************************************************************************************************************************************/
void modify_output_name(char *file_name, char *suffix, char *output_name)
{
    output_name = (char*)malloc(sizeof(char) * (strlen(file_name) + strlen(suffix)));
    if(!file_name)
        {
            printf("memmory allocation failed\n");
            return NULL;
        }
    strcpy(output_name, file_name);
    strcat(output_name, suffix);
}