/*
 ============================================================================
 ============================= Maman 14 =====================================
 File Name   : consts.h
 Semester    : 2017b
 Description : This file contains the constants for the project
 ============================================================================
*/

#ifndef CONSTS_H_
#define CONSTS_H_

/* some global constants */
#define ERROR -1
#define NOT_EXIST -2
#define VALID 1
#define TRUE 0
#define LINE_LENGTH 81
#define LABEL_MAX_LEN 30
#define COMMENT_FLAG ';'
#define INIT_IC 100

/* operation types and  addressing_method*/
#ifndef  ADDRESSING_METHOD_1
#define ADDRESSING_METHOD_1
typedef enum {EMPTY=-1,IMMEDIATELY=0,DIRECT=1,MATRIX=2,REG_DIRECT=3} addressing_method;
#endif
/* operation position: source / destination */
#define SOURCE 1
#define DESTINATION 2
#endif /* CONSTS_H_ */

#ifndef SWITCHER_1
#define SWITCHER_1

typedef enum {ON,OFF,WAIT} SWITCHER;
typedef enum {DATA=0,STRING=1,MATRIXF=2,ENTRY=3,EXTERN=4} DIRECRIVE_FUNCTION;

#endif

