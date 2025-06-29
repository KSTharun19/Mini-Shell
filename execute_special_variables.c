#include "main.h"
extern int exit_value;
void execute_special_variables(char *input)
{
    //ARRAY TO STORE PATH
    char cwd[PATH_MAX];
    
    if(!strcmp(input, "echo $?"))
    {
       printf("%d\n", exit_value);
    }
    else if(!strcmp(input, "echo $$"))
    {
        printf("%d\n", getpid());
    }
    else if(!strcmp(input, "echo $SHELL"))
    {
        if(getcwd(cwd, sizeof(cwd)) != NULL)
        {
           printf("%s\n", cwd);
        }
    }
    else
    {
        printf("%s\n", input+5);
    }
}