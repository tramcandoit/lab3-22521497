#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_KEY 0x1706
#define SHM_SIZE 10

typedef struct {
    int data[SHM_SIZE];
    int cnt;
} SharedMemory;

void producer(SharedMemory *shm) {
    srand(time(NULL));
    while (1) {
        int num= rand() % 11+10;

        if (shm->cnt < SHM_SIZE) {
            shm->data[shm->cnt] = num;
            printf("Produced: %d\n", num);
            shm->cnt++;
        }
        else {
            printf("Buffer is full\n");
            sleep(1);
        }
        sleep(1);
    }
}

void consumer(SharedMemory *shm) {
    int sum = 0;
    while (1) {
        if (shm->cnt > 0) {
            int num = shm->data[shm->cnt-1];
            printf("Consumed: %d\n", num);
            sum+=num;
            shm->cnt--;
        }
        else {
            printf("Buffer is empty\n");
            sleep(1);
        }
        if (sum>100) break;
    }
    printf("Sum = %d\n", sum);
}

int main() {
    int shmid;
    SharedMemory *shm;

    shmid = shmget(SHM_KEY, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    shm = (SharedMemory *) shmat(shmid, NULL, 0);
    if (shm == (void *)-1) {
        perror("shmat");
        return 1;
    }

    shm->cnt = 0;

    pid_t pid= fork();
    if (pid==-1) {
        perror("fork");
        exit(1);
    }
    else if (pid==0) {
        consumer(shm);
    }
    else {
        producer(shm);
        wait(NULL);
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
    }
}
