#include <stdio.h>

first_run(file);
second_run(file);


/*****************************************************************************************************************************************************
	*Function: int main(...)
	*Input: file_name.as
	*Return: 0 on success
	*Description: read assembly file and decodes it to machine language, Outputs 3 organized file_name.ob/ent/ext in a "strange 4-base" display. 
*****************************************************************************************************************************************************/
int main ( int argc, char *argv[] )//לבדוק אם הוראה לקובץ יחיד או שניתן להכניס יותר - בסרטון של דני דובר על 2 קבצים יחד בשורת פקודה.
{   
    int ic = dc = error = 0;
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
           error = first_run(file, &dc, &ic);
            if(error)
            {
                //free and clear functions without creating any file
                fclose(file);
            }
            rewind(file);
            second_run(file, &dc, &ic);
            if(error)
            {
                //free and clear functions without creating any file
                fclose(file);
            }   
            //create the needed files and free and clear data.
        }
        fclose(file);
    }
return 0;
}