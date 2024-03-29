#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/wait.h>

#define RED "\033[0;31m"                               //colors defined
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

#define args 10
#define BUFFER_SIZE 100

void print_prompt() {
  char hostname[BUFFER_SIZE];
  char *login = getlogin();
  gethostname(hostname, BUFFER_SIZE);
  printf(GREEN"%s@%s\033[0m"RED":~$ \033[0m", hostname, login);
}

int main(){
  char input[BUFFER_SIZE];
  char *arguments[args];

  while (1){
    print_prompt();
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = '\0';

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

    if(strcmp(arguments[0], "exit") == 0) {
      printf(YELLOW"Dovidjenja prijatno\033[0m\n");
      break;
    }
    else if(strcmp(arguments[0], "name") == 0) {
      printf("Shell Name: "RED"TOMMY SHELLBY \033[0m\n");
      continue;
    }
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
    else if(strcmp(arguments[0], "grep") == 0) {
      // Implement grep functionality
      // ...
    }
    else if(strcmp(arguments[0], "df") == 0) {
      // Implement df functionality
      // ...
    }
    else if(strcmp(arguments[0], "cmatrix") == 0) {
      // Implement cmatrix functionality
      // ...
    }
    else if(strcmp(arguments[0], "cowsay") == 0) {
      // Implement cowsay functionality
      // ...
    }
    else if(strcmp(arguments[0], "cd") == 0) {
      if(arguments[1] != NULL) {
        if(chdir(arguments[1]) == -1) {
          perror("cd");
        }
      }
      else {
        chdir(getenv("HOME"));
      }
      continue;
    }
    else if(strcmp(arguments[0], "rm") == 0) {
      if(arguments[1] == NULL) {
        fprintf(stderr, "Usage: rm FILE\n");
        continue;
      }
      if(unlink(arguments[1]) == -1) {
        perror("rm");
      }
      continue;
    }
    else {
      pid_t pid = fork();
      if (pid == 0) { // child process
        execvp(arguments[0], arguments);
        perror("Command not found\n");
        exit(1);
      }
      else if (pid > 0) { // parent process
        waitpid(pid, NULL, 0);
      }
    }
  }
  return 0;
}
