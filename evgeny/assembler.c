#include <stdio.h>
#define LINE_LENGTH 81

first_run(char []buff);
second_run(char []buff);


/*****************************************************************************************************************************************************
	*Function: int main(...)
	*Input: file_name.as
	*Return: 0 on success
	*Description: read assembly file and decodes it to machine language and outputs 3 organized file_name.ob/ent/ext in a "strange 4-base" display. 
*****************************************************************************************************************************************************/
int main ( int argc, char *argv[] )
{   
    char buff[LINE_LENGTH];
    if ( argc != 2 ) /* argc should be 2 for correct execution */
    {
        /* We print argv[0] assuming it is the program name */
        printf( "usage: %s filename\n", argv[0] );
    }
    else 
    {
        /* We assume argv[1] is a filename to open*/
        FILE *file = fopen( argv[1], "r" );

        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == NULL )
        {
            printf( "Could not open file\n" );
            exit(1); 
        }
        else /*begin the 2 stage processing*/
        {
            while(fgets( buff, LINE_LENGTH, (FILE*)file ))
                first_run(buff);
            rewind(file);
            while(fgets( buff, LINE_LENGTH, (FILE*)file ))
                second_run(buff);
            if(!ERROR)
            {
                //free and clear all data without creating any file
                fclose(file);
            }   
            //create the needed files and free and clear data.
        }
        fclose(file);
    }
return 0;
}