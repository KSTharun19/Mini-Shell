#include "main.h"
//FUNCTION TO CHECK< IS COMMANDS ARE EXTERNAL, INTERNAL OR ECHO

char *internal_cmd[] = {"cd", "pwd", "jobs", "fg", "bg", NULL};

char *get_word(char *input)
{
    int i = 0, j = 0;
    while(input[i] != '\0' && input[i] != ' ')
    {
        i++;
    }

    char *word = (char*)malloc(i+1);
    if(word == NULL)
    {
        return NULL;
    }
    for (j = 0; j < i; j++) 
    {
        word[j] = input[j];
    }
    word[j] = '\0';

    return word;
}

//PROGRAM TO CHECK THE COMMAND TYPE
int check_command_type(char *input)
{
    char *cmd = get_word(input);

   for(int i = 0; internal_cmd[i] != NULL; i++)
   {
    if(!strcmp(cmd, internal_cmd[i]))
    {
        return INTERNAL;
    } 
   }

    if(!strcmp(cmd ,"echo"))
    {
        return ECHO;
    }
    else
    {
        return EXTERNAL;
    }
}