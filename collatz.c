#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

void collatz(int n, int *buffer, int *buffer_length) {
    int i = *buffer_length;
    buffer[i++] = n;
    while (n != 1) {
        if (n % 2 == 0) n = n / 2;
        else n = 3 * n + 1;
        buffer[i++] = n;
    }
    *buffer_length = i;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "sử dụng: %s <số_nguyên_dương>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Vui lòng nhập số nguyên dương.\n");
        return 1;
    }

    int shmid;
    int *buffer;
    int *buffer_length;

    shmid = shmget (IPC_PRIVATE, BUFFER_SIZE * sizeof(int), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    buffer = (int *) shmat(shmid, NULL, 0);
    if (buffer == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    buffer_length = buffer + 1;
    *buffer_length = 0;

    pid_t pid = fork(); 
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        collatz(n, buffer, buffer_length);
        shmdt (buffer);
        exit(0);
    } else {
        wait(NULL);
        printf("Collatz sequence: ");
    for (int i = 0; i < *buffer_length; i++) {
        printf("%d ", buffer[i]);
    }
        printf("\n");
        shmdt(buffer);
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}
