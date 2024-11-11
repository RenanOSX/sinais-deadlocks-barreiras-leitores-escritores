#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_cond_t cond;
int ready = 0;

void* waiter(void* arg) {
    pthread_mutex_lock(&lock);
    while (!ready)
        pthread_cond_wait(&cond, &lock);
    printf("Thread %ld: sinal recebido, prosseguindo.\n", (long)arg);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* sender(void* arg) {
    pthread_mutex_lock(&lock);
    ready = 1;
    printf("Thread %ld: enviando sinal.\n", (long)arg);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&t1, NULL, waiter, (void*)1);
    pthread_create(&t2, NULL, sender, (void*)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
}
