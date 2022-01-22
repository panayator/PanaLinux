#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SHELL_TOK_BUFSIZE 256
#define SHELL_TOK_DELIM " >|"
#define SHELL_RL_BUFSIZE 1024
#define PATH_TOKEN "/"

int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);           // definitions , declarations and library inclusions

char *builtin_str[] = 
{
  "cd",
  "help",                              // commands already built-in the basic lsh shell protocol
  "exit"
};

int(*builtin_func[]) (char **) = 
{
  &shell_cd,
  &shell_help,                        // corresponding functions of the already built-in commands
  &shell_exit
};

int shell_num_builtins() 
{
  return sizeof(builtin_str) / sizeof(char *);
}

int shell_cd(char **args)                                        // Function of the cd /PATH command format
{
  if (args[1] == NULL) 
  {
    fprintf(stderr,"Shell: expected argument to \"cd\"\n");
  }
  else 
  {
    if(chdir(args[1]) != 0)
    {
      perror("Shell");
    }
  }
  return 1;
}

int shell_help(char **args)                                               // Function of the help command
{
  int i;
  printf("--------\n");
  printf("**HELP**\n");
  printf("--------\n");
  printf("This is a shell based on a basic lsh shell protocol.\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following were already implemented:\n");
  for (i = 0; i < shell_num_builtins(); i++) 
  {
    printf("  %s\n", builtin_str[i]);
  }
  printf("All programs from '/bin/' folder are available for use.\n");
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int shell_exit(char **args)           // Function of the exit command
{
  return 0;                         
}

int shell_launch(char **args)                                 // Function that takes the arguments given by the user
{                                                             // and executes the appropriate commands depending
  char fd;                                                    // on the number and form of these arguments
  char a[20],b[20],c[20],*ret,**words,d[20],*rat,*bol;
  char p = '/';
  int i,lastpos,pathsize;
  strcpy(a,args[0]);
  if(strchr(args[0],p))
  {
    rat = strchr(a, p);
    bol = rat;
    while(bol)
    {
      for(i=0;i<strlen(rat);i++)
      {
        rat[i]=rat[i+1];
      }
      rat[strlen(rat)] = '\0';
      bol = strchr(rat, p);
    }
    strcpy(d,rat);
    if(args[1] == NULL)
    {
        if(strstr(d,".c"))
        {
          int fd[2], i;
          char line[100], buffer[100];
          char *args[] = {"gcc",d,NULL};
          int status;
          pipe(fd);

          pid_t pid = fork();
          if (pid < 0)
          {
            printf("Fork Failed\n");
            exit(-1);
          }
          else if(pid > 0)
          {
             do
             {
                  waitpid(pid, &status, WUNTRACED);
             }while (!WIFEXITED(status) && !WIFSIGNALED(status));
     
            pid_t pid = fork();
    
            if (pid < 0) 
            {
               printf("Fork Failed\n");
               exit(-1);
            }
            else if (pid > 0)
            {
              do 
              {
                waitpid(pid, &status, WUNTRACED);
              } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
            else 
            {
              execl("./a.out","a.out","1","2",NULL);
            }
          return 0;
        }
     }
     else  
      {
        execvp(args[0],args);
      }
    return 0;
    
        execl(args[0],d,NULL); 
        return 1;
    }
  }

  pid_t pid;
  int status;
  pid = fork();
  if (pid == 0)
  {
    //Child process
    if(args[1]==NULL)
    {
      char path[20];
      strcpy(path,"/bin/");
      strcat(path,args[0]);
      if(strstr(args[0],"man")){
           strcpy(path,"/usr/bin/");
           strcat(path,a);
      }
      execl(path ,a, (char *) 0);

      exit(EXIT_FAILURE);      
    }
    else
    {
       strcpy(b,args[1]);
       if(args[2]==NULL)
       {
          char *ret,*rit;
          char path[20];
          strcpy(c,args[1]);
          ret = strstr(c,".txt");
          if(strstr(args[0],"man"))
          {
                strcpy(path,"/usr/bin/");
                strcat(path,a);
                execvp(args[0],args);
          }
          if(ret != NULL)
          {              
            FILE *fp;
            fp = fopen(args[1],"w");
            close(fd);
           
            if((fd = open(c, O_RDWR | O_CREAT))==-1)
            { /*open the file */
                return 1;
            }
      
            char path[20];
            strcpy(path,"/bin/");
            strcat(path,args[0]);
            dup2(fd,STDOUT_FILENO);          /* copy the file descriptor fd into standard output */
            dup2(fd,STDERR_FILENO);          /* same, for the standard error */
            close(fd);                       /* close the file descriptor as we don't need it more  */
            execl( path ,a, (char *) 0 );
            exit(EXIT_FAILURE);      
          }
          else
          {
            char path[20],*rit;
            rit = strstr(c,"./");
        
            if(rit != NULL)
            {
                char d[20];
                int i;
                for(i = 2;i<(int)strlen(c);i++)
                {
                  d[i-2] = c[i];
                }
                printf("%s \n",d);
            }
            else
            {
              strcpy(path,"/bin/");
              strcat(path,args[0]);
              execl( path ,a,b, (char *) 0 );
              exit(EXIT_FAILURE);      
            }
          }
        }
        else
        {
          char *rit;
          strcpy(c,args[2]);
          rit = strstr(c,"./");
          if(rit != NULL)
          {
            char d[20];
            int i;
            for(i = 2;i<(int)strlen(c);i++)
            {
              d[i-2] = c[i];
            }
            printf("%s \n",d);
          }
          else
          {
            FILE *fp;
            fp = fopen(args[2],"w");                 /* create and open the file */
            close(fd);

            if((fd = open(c, O_RDWR | O_CREAT))==-1)
            { 
              return 1;                             
            }

            char path[20];
            strcpy(path,"/bin/");
            strcat(path,args[0]);
            dup2(fd,STDOUT_FILENO);                 /*copy the file descriptor fd into standard output*/
            dup2(fd,STDERR_FILENO);                 /* same, for the standard error */
            close(fd);                              /* close the file descriptor as we don't need it more  */

            execl(path,a,b, (char *) 0 );

            exit(EXIT_FAILURE);
          }
        }
      }
    }
    else if(pid < 0)
    {
      // Error forking
      perror("Shell");
    } 
    else
    {
      // Parent process
      do 
      {
        waitpid(pid, &status, WUNTRACED);
      }while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } 
  return 1;
}

int shell_execute(char **args)                        // This function is needed in case one of the already built-in
{                                                     // commands is given by the user and guides the program to
  int i;                                              // execute them .
  if(args[0] == NULL) 
  {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < shell_num_builtins(); i++)
   {
    if (strcmp(args[0], builtin_str[i]) == 0)
    {
      return (*builtin_func[i])(args);
    }
  }
  return shell_launch(args);
}

char *shell_read_line(void)                                   // Puts the line of commands given by the user in
{                                                             // a dynamic two-dimensional array which the program
  int bufsize = SHELL_RL_BUFSIZE;                             // itself can edit accordingly .
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;
  if (!buffer)
  {
    fprintf(stderr, "Shell: allocation error\n");
    exit(EXIT_FAILURE);
  }
  while(1)
  {
    // Read a character
    c = getchar();
    if (c == EOF)
    {
      exit(EXIT_SUCCESS);
    }
    else if (c == '\n')
    {
      buffer[position] = '\0';
      return buffer;
    }
    else
    {
      buffer[position] = c;
    }
    position++;    
  }
}

char **shell_split_line(char *line)                          // Function that splits the line taken from the dynamic
{                                                            // array that was formed in the shell_read_line function
  int bufsize = SHELL_TOK_BUFSIZE, position = 0;             // if it finds any of the ' ' , '>' or '|' characters .
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;
  if (!tokens)
  {
    fprintf(stderr, "Shell: allocation error\n");
    exit(EXIT_FAILURE);
  }
  token = strtok(line, SHELL_TOK_DELIM);
  while (token != NULL)
  {
    tokens[position] = token;
    position++;    
    token = strtok(NULL, SHELL_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

void shell_loop(void)                       // Function that starts the basic loop of our program and actually runs
{                                           // the shell and executes the given commands by the user .
  char *command;
  char **args;
  int status;

  do 
  {
    printf("--> ");
    command = shell_read_line();
    args = shell_split_line(command);
    status = shell_execute(args);

    free(command);
    free(args);
  }while(status);
}

int main(int argc, char *argv[])
{
  // Load config files, if any.
  // Run command loop.
  shell_loop();
  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}