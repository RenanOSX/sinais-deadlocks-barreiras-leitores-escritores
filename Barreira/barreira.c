#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

pthread_barrier_t barrier;

void* thread_func(void* arg) {
    int thread_id = *(int*)arg;

    printf("Thread %d: Esperando pela barreira...\n", thread_id);

    pthread_barrier_wait(&barrier); // Espera na barreira

    printf("Thread %d: Passou pela barreira!\n", thread_id);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Inicializando a barreira com NUM_THREADS participantes
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Criando threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
    }

    // Esperando todas as threads passarem pela barreira
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruindo a barreira
    pthread_barrier_destroy(&barrier);

    return 0;
}
