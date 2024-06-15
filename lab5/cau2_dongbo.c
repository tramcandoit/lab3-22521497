#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int *a;
int size = 0;
int n;
sem_t sem;

void *producer(void *arg) {
    while (size < n) {
        sem_wait (&sem);
        int r = rand() % 100;
        a[size++] = r;
        printf("Process %d, size = %d\n", r, size);
        sem_post(&sem);
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) {
        sem_wait(&sem);
        if (size == 0) {
            printf("Nothing in array a\n");
            sem_post(&sem);
            return NULL;
        } else {
            int r = a[--size];
            printf("Consumed %d, size = %d\n", r, size);
        }
        sem_post(&sem);
    }
}

int main() {
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);

    a = (int*)malloc(n * sizeof(int));

    pthread_t t1, t2;
    sem_init(&sem, 0, 1);
    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, producer, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);
    free(a);

    return 0;
}