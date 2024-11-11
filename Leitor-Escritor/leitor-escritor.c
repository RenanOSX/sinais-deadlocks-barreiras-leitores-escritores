#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock;
pthread_mutex_t write_lock;
int shared_data = 0;
int reader_count = 0;

void* reader(void* arg) {
    pthread_mutex_lock(&lock);
    reader_count++;
    if (reader_count == 1) {
        pthread_mutex_lock(&write_lock);
    }
    pthread_mutex_unlock(&lock);
    
    printf("Leitor %ld: lendo valor %d\n", (long)arg, shared_data);

    pthread_mutex_lock(&lock);
    reader_count--;
    if (reader_count == 0) {
        pthread_mutex_unlock(&write_lock);
    }
    pthread_mutex_unlock(&lock);

    return NULL;
}

void* writer(void* arg) {
    pthread_mutex_lock(&write_lock);
    
    shared_data++;
    printf("Escritor %ld: escrevendo valor %d\n", (long)arg, shared_data);

    pthread_mutex_unlock(&write_lock);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&write_lock, NULL);

    for (long i = 0; i < 5; i++)
        pthread_create(&readers[i], NULL, reader, (void*)i);
    for (long i = 0; i < 2; i++)
        pthread_create(&writers[i], NULL, writer, (void*)i);

    for (int i = 0; i < 5; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(writers[i], NULL);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&write_lock);
    return 0;
}
