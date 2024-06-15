#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *a;
int size = 0;
int n;

void *producer(void *arg) {
    while (size < n) {
        int r = rand() % 100;
        a[size++] = r;
        printf("Process %d, size = %d\n", r, size);
    }
    return NULL;
}

void *consumer(void *arg) {
    while (1) 
    {
        if (size == 0) {
            printf("Nothing in array a\n");
            return NULL;
        } else {
            int r = a[--size];
            printf("Consumed %d, size = %d\n", r, size);
        }
    }
}

int main() {
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);

    a = (int*)malloc(n * sizeof(int));

    pthread_t t1, t2;
    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, producer, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    free(a);

    return 0;
}