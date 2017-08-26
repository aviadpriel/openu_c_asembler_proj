/*my functions prototypes*/
/*1*/
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

void output(char *file_name, binWordList *binWordHead, dataList *dataHead,extEntList *extEntHead)
{
    char *full_ent_name = NULL;
    char *full_ext_name = NULL;
    char *full_ob_name = NULL;
    File *fpent = NULL;
    File *fpext = NULL;
    File *fpob = NULL;
    if(labelsList->entry == ON)/*creates .ent file*/
    {
        if(!fpent)
        {
            modify_name(full_ent_name, strlen(file_name)+ENT_SUFFIX, file_name );
            fpent = fopen(full_ent_name,"w");
        }
    }
    else if(labelsList->external == ON)/*creates .ext file*/
    {
        if(!fpext)
        {
            modify_name(full_ext_name, strlen(file_name)+EXT_SUFFIX, file_name );
            fpent = fopen(full_ent_name,"w");
        }
    }
    if(!fpext)/*creates .ob file*/
    {
        modify_name(full_ext_name, strlen(file_name)+EXT_SUFFIX, file_name );
        fpent = fopen(full_ent_name,"w");
    }

}

/*2*/
void output_entry(char **file_name, int size, char *argv)
{
	char* full_name = new_string(strlen(file_name)+4);
	strcpy(full_name,file_name);
	strcat(full_name,".ent");

	FILE *fp = fopen(full_name,"w");

	HashNode *node;
	for_each_item(node,entry)
	{
		fprintf(fp,"%s \t\t %o\n",node->value,node->key);
	}

	free(full_name);
	fclose(fp);
}

/*3*/
void output_extern(char*file_name)
{
	char* full_name = new_string(strlen(file_name)+4);
	strcpy(full_name,file_name);
	strcat(full_name,".ext");

	FILE *fp = fopen(full_name,"w");

	HashNode *node;
	for_each_item(node,externals)
	{
		if(node->key != NOT_SET)
			fprintf(fp,"%s \t\t %o\n",node->value,node->key);
	}

	free(full_name);
	fclose(fp);
}

/*4*/
void output_commands(char*file_name,Output_Command* output,int IC,int DC)
{
	int i;
	char* full_name = new_string(200);
	strcpy(full_name,file_name);
	strcat(full_name,".ob");

	FILE *fp = fopen(full_name,"w");

	/* print the output commands and their linker letter (a/r/e) */
	fprintf(fp,"     \t  %o %o\n", IC,DC);
	for(i=0;i<IC;i++)
	{
		fprintf(fp,"%04o \t %06o \t %c\n", i, to16bit(output[i].bin_command), output[i].linker_data);
	}

	/* print the .data and .string information from the data array */
	for(i=IC; i<IC+DC; i++)
	{
		fprintf(fp,"%04o \t %06o\n", i, to16bit(data[i-IC]));
	}

	free(full_name);
	fclose(fp);
}

/*int to strange base for IC\DC*/
char *int_to_strange(int num)
{
	char adress[5]={"\0"};/*max memmory size is represented by 5 "digits"(256 == baaaa)*/
	char *base4 ={"abcd"};
	int i = --strlen(adress);
	static char *p_adress = adress;

	while(i)
	{
		adress[i] = num % 4;
		num /= 4;
		i--;
	}
	while(!p_adress && p_adress != '\0')/*maybe -1 better?*/
		p_adress++;
	if(p_adress == '\0')
		p_adress--;
	return p_adress
}