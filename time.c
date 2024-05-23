#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Sử dụng: %s <lệnh>\n", argv[0]);
        return 1;
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    else if (pid == 0) {
        if (execl("/bin/sh", "sh", "-c", argv[1], NULL) == -1) {
            perror("execl");
            exit(1);
        }
    }
    else {
        int status;
        waitpid(pid, &status, 0);
        gettimeofday(&end, NULL);
        double elapsedTime = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        printf("Thời gian thực thi: %.6f giây\n", elapsedTime);
    }
    return 0;
}
