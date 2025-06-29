#include "main.h"

//PROGRAM TO ADD THE JOB TO LIST
int add_to_list(job_list **jobs, int pid)
{
    job_list *new = (job_list*)malloc(sizeof(job_list));
    if(new == NULL)
    {
       printf("Failed to add job...\n");
       return FAILURE;
    }
    new->pid = pid;
    new->status = 0;
    new->next = *jobs;

    *jobs = new;

    return SUCCESS;
}

//PROGRAM TO REMOVE THE JOB LIST
int remove_job_from_list(job_list **jobs, int pid)
{
   job_list *temp = *jobs;
   job_list *prev = NULL;

   while(temp != NULL)
   {
     if(temp->pid == pid)
     {
        if(prev == NULL)
        {
            *jobs = temp->next; 
        }
        else
        {
            prev->next = temp->next;
        }
        free(temp);
        return SUCCESS;
     }
      prev = temp;
      temp = temp->next;
   }
   return FAILURE;
}

//PROGRAM TO PRINT THE JOB LIST
void print_jobs(job_list *jobs)
{
    job_list *temp = jobs;
    if(jobs == NULL)
    {
        printf("No recent stoped events\n");
        return;
    }
    while (temp)
    {
        printf("[%d]+         Stopped\n", temp->pid);
        temp = temp->next;
    }
    
}
