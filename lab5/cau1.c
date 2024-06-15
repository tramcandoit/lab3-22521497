#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

pthread_t tid[2];
sem_t sem;
int sells, products;
int MSSV = 1497;

void* ProcessA(void *arg) {
    while (sells < products) 
    {
        sem_wait(&sem);
        sells++;
        printf("Sells: %d\n", sells);
    }
    return NULL;
}

void* ProcessB(void *arg) {
    while (products < sells + MSSV) {
        products++;
        printf("Products: %d\n", products);
        sem_post(&sem); 
    }
    return NULL;
}

int main() {
    int i;
    sells = 0;
    products = 0;
    sem_init(&sem, 0, 0);

    pthread_create(&tid[1], NULL, ProcessA, NULL);
    pthread_create(&tid[0], NULL, ProcessB, NULL);

    for(i = 0; i < 2; i++) {
        pthread_join(tid[i], NULL);
    } 
    return 0;
}