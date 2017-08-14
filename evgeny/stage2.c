/*****************************************************************************************************************************************************
This is stage2 file
*****************************************************************************************************************************************************/
#include <stdio.h>
#include "stage2.h"
#include "general.h"
#include "errors.h"
#include "output.h" //optional

/****************************************************************************************************************************************************
	*Function: stage2
	*Input: FILE *file- pointer to file.as, data *data_head- pointer to the head of data list,
            lableList *label_head- pointer to the head of label list,codeList *code_head- pointer to the head of code list.
	*Return: 0 if no errors else non zero.
	*Description: runs the file.as second time to fill the remaining missing data.
****************************************************************************************************************************************************/
stage2(FILE *file, data *data_head, lableList *label_head,codeList *code_head)
{
    int ic = 0;//ic needs to be initialized to the begining (not always 0)
    char *field, label[30];//magic number here

    while(fgets(line, LINE_LEN, file))
    {
        switch(is_label(field = next_field(line), label_head))
        {
            case true: 
                field = next_field(line);
                break;
            case false:
                break;
            default:
                printf("line %d: label \"%s\" does not exist\n",/*line index here*/, field);//missing line indicator
                break;
        }
        if(is_directing(field)
        {
            //switch label flag is_entry to on
            continue;
        }
        //קידוד אופרנדים סעיף 7 במעבר שני.

    }
    if(errors, STAGE2)/*found errors*/
    {
        output(errors, STAGE2);// STAGE2 is enum for error type.
        return -1 //magic number
    }
    //write here create output files functions
    return 0;
}/*stage2*/

int is_directing(char *field)
{
    if(field != '.')
        return false;
    else
        {
            field ++; /*exclude '.'*/
            if(strcmp("entry", field))
                {
                    curr.is_entry = 1;//magic number here.
                }
        }
    return true;
}/*is_directing*/


/****************************************************************************************************************************************************
	*Function: is_label
	*Input: char field- next word of current line, labelList *head- pointer to lableList head.
	*Return: true (1) if found, false (0) if not a label, NOT_EXIST (-2) if label wasn't declared.
	*Description: checks if the field is label and declared.
****************************************************************************************************************************************************/
int is_label(char *field, labelList *head)
{
    labelList *curr = head;
    if(field[strlen(field - 1)] == ':')/*field is label*/
    {
        field[strlen(field) - 1] = '\0'; /*exclude ':' of label*/
        while(curr != NULL)
        {
            if(strcmp(curr.name, field))
              return true;/*true == 1*/
            else if(!curr)
                return NOT_EXIST;
            curr = curr.next;
        }
    }
    return false;//false == 0
}/*is_label*/
