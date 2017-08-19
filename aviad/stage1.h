/*****************************************************************************************************************************************************
This is stage1 header file
what to include inside wrapper? 
- new data types
- stage1 public functions prototype
*****************************************************************************************************************************************************/

#ifndef STAGE1_H
#define STAGE1_H

//write evreythig needed for stage1 header
#include "general.h"

int dataF(char *command, char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line);
int stringF(char *command,char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line);
int matF(char *command,char *label,int *dc,dataList **dataHead,labelsList **labelsHead,int line);
int externF(char *buff,char *label,labelsList **labelsHead,int line);
int entryF(char * buff,char *label,int line);
void updateDataLabel(labelsList **labelsHead,int curIc);
void updateDataList(dataList **dataHead,int curIc);

#endif /*end of STAGE1_H*/