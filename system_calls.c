#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    // fork()
    pid = fork();

    if (pid < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process

        // exec()
        if (execl("/bin/ls", "ls", "-l", NULL) == -1) {
            perror("exec() failed");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process

        // wait()
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child process exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally\n");
        }
    }

    return 0;
}