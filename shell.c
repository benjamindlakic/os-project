// necessary libraries needed for the code
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>          
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/wait.h>

// colors defined for the shell
#define RED "\033[0;31m"                               
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m" 
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

#define args 10
#define BUFFER_SIZE 100

// function to print the prompt
void print_prompt() {
  char hostname[BUFFER_SIZE];
  char *login = getlogin();
  gethostname(hostname, BUFFER_SIZE);
  printf(GREEN"%s@%s\033[0m"RED":~$ \033[0m", hostname, login);
}

// main function
int main(){

  char input[BUFFER_SIZE];
  char *arguments[args];

  // loop to keep the shell running
  while (1){
    print_prompt();
    // read the input from the user
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0';

    // if the input is empty, continue
    if(strcmp(input, "") == 0) {
      continue;
    }

    //tokens
    char *token = strtok(input, " ");
    int i = 0;
    while(token != NULL && i < args) {
      arguments[i] = token;
      token = strtok(NULL, " ");
      i++;
    }
    arguments[i]=NULL; //last element always has to be null for execvp()

    // if the input is exit, break the loop
    if(strcmp(arguments[0], "exit") == 0) {
      printf(RED"Good luck lad.\033[0m\n");
      break;
    }
    // if the input is name, print the name of the shell
    else if(strcmp(arguments[0], "name") == 0) {
      printf("Shell Name: "RED"TOMMY SHELLBY \033[0m\n");
      continue;
    }

    // if the input is wc, count the number of lines, words and characters in the file
    else if(strcmp(arguments[0], "wc") == 0) {
      if(arguments[1] == NULL) {
        fprintf(stderr, "Usage: wc FILE\n");
        continue;
      }

      FILE *file = fopen(arguments[1], "r");
      if(file == NULL) {
        perror("wc");
        continue;
      }

      int lines = 0;
      int words = 0;
      int characters = 0;
      int in_word = 0;
      char ch;

      while((ch = fgetc(file)) != EOF) {
        characters++;

        if(ch == '\n') {
          lines++;
        }

        if(ch == ' ' || ch == '\t' || ch == '\n') {
          in_word = 0;
        }
        else if(in_word == 0) {
          in_word = 1;
          words++;
        }
      }

      fclose(file);

      printf("Lines: %d\n", lines);
      printf("Words: %d\n", words);
      printf("Characters: %d\n", characters);
      continue;
    }

    // if the input is grep, search for a pattern in the file
    else if(strcmp(arguments[0], "grep") == 0) {
      if(arguments[1] == NULL || arguments[2] == NULL) {
        fprintf(stderr, "Usage: grep PATTERN FILE\n");
        continue;
      }

      char command[BUFFER_SIZE];
      snprintf(command, BUFFER_SIZE, "grep %s %s", arguments[1], arguments[2]);

      FILE *output = popen(command, "r");
      if(output == NULL) {
        perror("grep");
        continue;
      }

      char line[BUFFER_SIZE];
      while(fgets(line, BUFFER_SIZE, output) != NULL) {
        printf("%s", line);
      }

      pclose(output);
      continue;
    }

    // if the input is df, display the disk space
    else if(strcmp(arguments[0], "df") == 0) {
      FILE *output = popen("df -h", "r");
      if(output == NULL) {
        perror("df");
        continue;
      }

      char line[BUFFER_SIZE];
      while(fgets(line, BUFFER_SIZE, output) != NULL) {
        printf("%s", line);
      }

      pclose(output);
      continue;
    }

    // if the input is cmatrix, display the matrix, but failed to implement this one 
    else if(strcmp(arguments[0], "cmatrix") == 0) {
      pid_t pid = fork();
      if (pid == 0) {
        execlp("cmatrix", "cmatrix", NULL);
        perror("cmatrix");
        exit(1);
      } else if (pid < 0) {
        perror("fork");
      } else {
        wait(NULL);
      }
      continue;
    }

    // if the input is ls, display the files in the directory
    else if(strcmp(arguments[0], "ls") == 0) {
      if(arguments[1] == NULL) {
        system("ls");
      }
      else {
        char command[BUFFER_SIZE];
        snprintf(command, BUFFER_SIZE, "ls %s", arguments[1]);
        system(command);
      }
      continue;
    }
    
    // if the input is pwd, display the current working directory
    else if(strcmp(arguments[0], "pwd") == 0) {
      char cwd[BUFFER_SIZE];
      if(getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("pwd");
      }
      else {
        printf("%s\n", cwd);
      }
      continue;
    }

    // if the input is date, display the current date
    else if(strcmp(arguments[0], "date") == 0) {
      pid_t pid = fork();
      if (pid == 0) {
        execlp("date", "date", NULL);
        perror("date");
        exit(1);
      } else if (pid < 0) {
        perror("fork");
      } else {
        wait(NULL);
      }
      continue;
    }

    // if the input is cd, change the directory
    else if(strcmp(arguments[0], "cd") == 0) {
      if(arguments[1] == NULL) {
        fprintf(stderr, "Usage: cd DIRECTORY\n");
        continue;
      }
      if(chdir(arguments[1]) == -1) {
        perror("cd");
      }
      continue;
    }
    
    // if the input is not any of the above, execute the command
    else {
      pid_t pid = fork();
      if (pid == 0) { // child process
        // Redirect output to a file if ">" is present in the command
        int output_fd = -1;
        for (int j = 0; arguments[j] != NULL; j++) {
          if (strcmp(arguments[j], ">") == 0) {
            output_fd = open(arguments[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
              perror("open");
              exit(1);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
            break;
          }
        }
        execvp(arguments[0], arguments);
        perror(RED"Command not found in our shelly.\n");
        exit(1);
      }
      else if (pid > 0) { // parent process
        waitpid(pid, NULL, 0);
      }
    }
  }
  return 0;
}
