#include "main.h"


job_list *job = NULL;
int job_counter = 0;
//VARIABLE TO TRACK PRCESSES
int fg_pid = -1;
//CUSTOMIZIABLE PROMPT
int exit_value = 0;

void signal_handler(int signum)
{
  if(fg_pid > 0)
  {
    if(signum == SIGINT)
    {
       kill(fg_pid, SIGINT);
       remove_job_from_list(&job, fg_pid);
       job_counter -= 1;
    }
    else if(signum == SIGTSTP)
    {
       kill(fg_pid, SIGTSTP);
       printf("\n[%d]+ %15s\n", fg_pid, "Stopped");
       job_counter += 1;
       add_to_list(&job, fg_pid);
    }
  }
  else
  {
    printf("\nminishell$ ");
    fflush(stdout);
  }
}
//STRUCT VARIABLE TO STORE PENDING PROCESSES AND STOPPED PROCESSE
int main()
{
    char input[100], prompt[100] = "minishell$";
    int short com_type;
   
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
   while(1)
   {
      printf("%s ", prompt);
      fgets(input, 100, stdin);
      input[strlen(input) - 1] = '\0';
      
      if(!strcmp(input, "exit"))
      {
        return SUCCESS;
      }
      else if(!strncmp(input, "PS1=", 4) && strchr(input, ' ') == NULL)
      { 
        strcpy(prompt, input+4);
      }
      else if((com_type = check_command_type(input)) == EXTERNAL)
      {
        exit_value = execute_external_command(input);
      }
      else if(com_type == INTERNAL)
      {
        exit_value = execute_internal_command(input);
      }
      else if(com_type == ECHO)
      {
        execute_special_variables(input);
      }
      else
      {
        printf("Command '%s' not found\n", strchr(input, ' '));
      }
   } 
}
