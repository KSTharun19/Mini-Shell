#include "main.h"

extern int exit_value;
extern int fg_pid;
int execute_external_command(char *input)
{
   char *commands[10][10];

   int cmd_count = 0, arg_index = 0;


   char *token = strtok(input, " ");
   while (token != NULL)
   {
      if(strcmp(token, "|") == 0)
      {
         commands[cmd_count][arg_index] = NULL;
         cmd_count++;
         arg_index = 0;
      }
      else
      {
         commands[cmd_count][arg_index++] = token;
      }
      token = strtok(NULL, " ");
   }
   commands[cmd_count][arg_index] = NULL;
   cmd_count++;

   if (cmd_count < 1)
   {
     return FAILURE;
   }
   if (cmd_count == 1)
   {
      int pid = fork();
      if (pid == -1)
      {
         perror("fork failed");
         return FAILURE;
      }
      if (pid == 0) // Child process
      {
         execvp(commands[0][0], commands[0]);
         perror("execvp failed");
         exit(EXIT_FAILURE);
      }
        // Parent waits for child
      fg_pid = pid;
      int status;
      if(waitpid(pid, &status, WUNTRACED) == -1)
        {
          perror("waitpid");
          fg_pid = -1;
          return FAILURE;
        }
        fg_pid = -1;
        if(WIFEXITED(status))
        {
          printf("The child %d is terminated normally with the code %d\n", pid, (exit_value = WEXITSTATUS(status)));
        }
        else if(WIFSIGNALED(status))
        {
          exit_value = 128 + WTERMSIG(status);
          printf("The child %d is terminated abnormally by recieving the signal %d\n", pid, WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
          exit_value = 128 + WSTOPSIG(status); 
          printf("The child %d is stopped by recieving the signal %d\n", pid, WSTOPSIG(status));
        }
      return exit_value;
   }

   int pipes[cmd_count-1][2];

   for(int i = 0; i < cmd_count; i++)
   {
      if(pipe(pipes[i]) == -1)
      {
         perror("pipe failed");
         return FAILURE;
      }
   }

   int pids[cmd_count];
   
   int pid_copy;
   for(int i = 0; i<cmd_count; i++)
   {
      int pid = fork();
      pid_copy = pid;

      if(pid == -1)
      {
         perror("fork failed");
         return FAILURE;
      }

      if(pid == 0)
      {
         if(i > 0)
         {
            dup2(pipes[i - 1][0], 0);
         }
         if(i < cmd_count - 1)
         {
            dup2(pipes[i][1], 1);
         }

         for(int j = 0; j < cmd_count - 1; j++)
         {
            close(pipes[j][0]);
            close(pipes[j][1]);
         }

         execvp(commands[i][0], commands[i]);

         perror("return failed");
         exit(EXIT_FAILURE);
      }
      pids[i] = pid;
   }
   
     for(int i = 0; i < cmd_count - 1; i++)
     {
        close(pipes[i][0]);
        close(pipes[i][1]);
     }

     int status;
     fg_pid =pid_copy;
     for(int i = 0; i < cmd_count; i++)
     {
        if(waitpid(pids[i], &status, WUNTRACED) == -1)
        {
          perror("waitpid");
          fg_pid = -1;
          return FAILURE;
        }
        fg_pid = -1;
        if(WIFEXITED(status))
        {
          printf("The child %d is terminated normally with the code %d\n", pids[i], (exit_value = WEXITSTATUS(status)));
        }
        else if(WIFSIGNALED(status))
        {
          exit_value = 128 + WTERMSIG(status);
          printf("The child %d is terminated abnormally by recieving the signal %d\n", pids[i], WTERMSIG(status));
        }
        else if(WIFSTOPPED(status))
        {
          exit_value = 128 + WSTOPSIG(status); 
          printf("The child %d is stopped by recieving the signal %d\n", pids[i], WSTOPSIG(status));
        }
     }
     return exit_value;
}
