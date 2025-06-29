#ifndef MAIN_H
#define MAIN_H

enum
{
  SUCCESS =  0,
  FAILURE = 1,
  INTERNAL,
  EXTERNAL,
  ECHO
};

typedef struct job
{
  int pid;
  int status;
  struct job *next;
}job_list;


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<errno.h>
#include <termios.h> 
#include <linux/limits.h>


int execute_external_command(char *input);
int execute_internal_command(char *input);
void execute_special_variables(char *input);
int check_command_type(char *input);
char *get_word(char *input);

//FUNCTIONS FOR LIST MECHANISM
int add_to_list(job_list **jobs, int pid);
int remove_job_from_list(job_list **jobs, int pid);
void print_jobs(job_list *jobs);


#endif