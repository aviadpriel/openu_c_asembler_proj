/*****************************************************************************************************************************************************
This is stage2 header file
what to include inside wrapper? 
- new data types
- stage2 public functions prototype
*****************************************************************************************************************************************************/

#ifndef STAGE2_H
#define STAGE2_H

//write evreythig needed for stage2 header
#include "general.h"
#include "errors.h"


int is_label(char *field, labelList *head, labelList curr);
int is_directing(char *field, char *label);
stage2(FILE *file, data *data_head, lableList *label_head,codeList *code_head);


#endif/*end of STAGE2_H*/