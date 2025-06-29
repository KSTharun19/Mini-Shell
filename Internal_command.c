#include "main.h"

extern job_list *job;

int execute_internal_command(char *input)
{
    char cwd[PATH_MAX];
        //EXECUTING THE GIVEN COMMAND
    if(!strncmp(input, "cd", 2))
    {
        char *path = input + 2;

        if (*path == '\0') 
        {
            path = getenv("HOME");  // If "cd" alone, go to HOME
        }
        else if (*path == ' ') 
        {
            path++;  // Skip the space after "cd"
        }
        else 
        {
            printf("Invalid command: %s\n", input);
            return FAILURE;
        }
        int res = chdir(path);
        if(res == 0)
        {
            return SUCCESS;
        }
        perror("cd");
        return FAILURE;
    }
    else if(!strcmp(input, "pwd"))
    {
        if(getcwd(cwd, sizeof(cwd)) != NULL)
        {
           printf("%s\n", cwd);
        }
        else
        {
           perror("getcwd failed");
           return FAILURE;
        }
        return SUCCESS;
    }
    else if(!strcmp(input, "jobs"))
    {
       print_jobs(job); 
       return SUCCESS;
    }
    else if (!strcmp(input, "fg")) 
    {
      if (job == NULL)
      {
         printf("No command running\n");
         return 1;
      }
      int status;
      kill(job->pid, SIGCONT); 
      waitpid(job->pid, &status, WUNTRACED); 

       // If job stopped again, keep it in the list
      if (WIFSTOPPED(status)) 
      {
         printf("Job [%d] stopped again\n", job->pid);
      } else 
      {
        remove_job_from_list(&job, job->pid); 
      }
    }
    else if(!strcmp(input, "bg"))
    {
       if (!job->pid)
       {
          printf("bg: No background job to resume\n");
          return FAILURE;
       } 

       int pid2 = job->pid;
       kill(pid2, SIGCONT);

       printf("Job [%d] resumed in background\n", pid2);
       remove_job_from_list(&job, pid2);
       return SUCCESS;
    }
    return FAILURE;
}