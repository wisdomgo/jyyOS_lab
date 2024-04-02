#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h> // For intptr_t

#define LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))

const int N = 7;

struct Edge {
    int from, to;
    pthread_mutex_t mutex;
} edges[] = {
    {1, 2},
    {2, 3},
    {2, 4},
    {2, 5},
    {4, 6},
    {5, 6},
    {4, 7},
};

void* T_worker(void* arg) {
    int id = (int)(intptr_t)arg;

    for (int i = 0; i < LENGTH(edges); i++) {
        struct Edge *e = &edges[i];
        if (e->to == id) {
            pthread_mutex_lock(&e->mutex);
        }
    }

    printf("Start %d\n", id);
    sleep(1);
    printf("End %d\n", id);
    sleep(1);

    for (int i = 0; i < LENGTH(edges); i++) {
        struct Edge *e = &edges[i];
        if (e->from == id) {
            pthread_mutex_unlock(&e->mutex);
        }
    }

    return NULL;
}

int main() {
    pthread_t threads[N];

    // Initialize mutexes
    for (int i = 0; i < LENGTH(edges); i++) {
        pthread_mutex_init(&edges[i].mutex, NULL);
    }

    // Lock all mutexes to prevent any worker from starting
    for (int i = 0; i < LENGTH(edges); i++) {
        pthread_mutex_lock(&edges[i].mutex);
    }

    // Start workers
    for (int i = 1; i <= N; i++) {
        pthread_create(&threads[i-1], NULL, T_worker, (void*)(intptr_t)i);
    }

    // Unlock to start the work
    for (int i = 0; i < LENGTH(edges); i++) {
        pthread_mutex_unlock(&edges[i].mutex);
    }

    // Wait for all workers to finish
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up mutexes
    for (int i = 0; i < LENGTH(edges); i++) {
        pthread_mutex_destroy(&edges[i].mutex);
    }

    return 0;
}
