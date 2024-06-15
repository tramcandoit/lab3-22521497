#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int x1, x2, x3, x4, x5, x6, w, v, y, z, ans;
pthread_mutex_t mutex_v, mutex_wy, mutex_yz;

void calculate_w() {
    w = x1 * x2;
}

void calculate_v() {
    v = x3 * x4;
    pthread_mutex_unlock(&mutex_v);
}

void calculate_y() {
    pthread_mutex_lock(&mutex_v);
    y = v * x5;
    pthread_mutex_unlock(&mutex_wy);
}

void calculate_z() {
    pthread_mutex_lock(&mutex_v);
    z = v * x6;
    pthread_mutex_unlock(&mutex_yz);
}

void calculate_ans() {
     pthread_mutex_lock(&mutex_wy);
     pthread_mutex_lock(&mutex_yz);
     ans = y + z;
     printf("ans = %d\n", ans);
}

int main() {
    printf("Nhap gia tri cho x1, x2, x3, x4, x5, x6: ");
    scanf("%d %d %d %d %d %d", &x1, &x2, &x3, &x4, &x5, &x6);

    pthread_mutex_init(&mutex_v, NULL);
    pthread_mutex_init(&mutex_wy, NULL);
    pthread_mutex_init(&mutex_yz, NULL);

    pthread_t thread_w, thread_v, thread_y, thread_z, thread_ans;
    pthread_create(&thread_w, NULL, (void *)calculate_w, NULL);
    pthread_create(&thread_v, NULL, (void *)calculate_v, NULL);
    pthread_create(&thread_y, NULL, (void *)calculate_y, NULL);
    pthread_create(&thread_z, NULL, (void *)calculate_z, NULL);
    pthread_create(&thread_ans, NULL, (void *)calculate_ans, NULL);
    
    pthread_join(thread_w, NULL);
    pthread_join(thread_v, NULL);
    pthread_join(thread_y, NULL);
    pthread_join(thread_z, NULL);
    pthread_join(thread_ans, NULL);

    pthread_mutex_destroy(&mutex_v);
    pthread_mutex_destroy(&mutex_wy);
    pthread_mutex_destroy(&mutex_yz);

    return 0;
}