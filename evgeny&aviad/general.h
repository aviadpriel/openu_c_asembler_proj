/*****************************************************************************************************************************************************
This is general header file.
what to include inside wrapper? 
- new data types
- general public functions prototype
*****************************************************************************************************************************************************/
#ifndef GENERAL_H
#define GENERAL_H

enum symbol_flag {}
/*****************************************************************************************************************************************************

*****************************************************************************************************************************************************/
typedef struct machine_code_node
{
    char psw[WORD_SIZE];/*assemblers base line*/
    machine_code *next;/*pointer to next assemblers base line*/
    binWord *nextDown;/*pointer to next word (if needed) related to this psw */
    int commandLineCnt;/*number line of the original file line*/

}machine_code_node;

/*****************************************************************************************************************************************************

*****************************************************************************************************************************************************/
typedef struct word 
{
char wordbase[WORD_SIZE];
int is_encoded : 1;/*1- this word is already decoden to binary number, 0- not decoded yet*/
word *next;/*next word related to the psw (if needed)*/
int IC;
}word;

#endif /*end of GENERAL_H*/