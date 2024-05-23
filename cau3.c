#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void handle_interrupt(int sig) {
    printf("count.sh has stopped\n");
    exit(0);
}

int main() {
    pid_t pid;
    printf( "Welcome to IT007, I am 22520023!\n");
    signal(SIGINT, handle_interrupt);
    pid = fork();
    if (pid < 0) {
        perror ("fork");
        return 1;
    }
    else if (pid == 0) {
        execl("/bin/bash", "bash", "-c", "./count.sh 120", NULL);
        perror("execl");
        exit(1);
    }
    else {
        int status;
        waitpid(pid, &status, 0);
    }
    return 0;
}
