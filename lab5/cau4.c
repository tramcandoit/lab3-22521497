#include <stdio.h>
#include <pthread.h>

int x = 0;
pthread_mutex_t mutex;

void *processA(void *mess) {
    while(1) {
        pthread_mutex_lock(&mutex);
        x = x + 1;
        if (x == 20) x = 0;
        printf("A = %d\n", x);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *processB(void *mess) {
    while(1) {
        pthread_mutex_lock(&mutex);
        x = x + 1;
        if (x == 20) x = 0;
        printf("B = %d\n", x);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t pA, pB;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&pA, NULL, &processA, NULL);
    pthread_create(&pB, NULL, &processB, NULL);

    pthread_join(pA, NULL);
    pthread_join(pB, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}