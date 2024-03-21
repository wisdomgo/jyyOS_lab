#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

long sum = 0;
long N = 10000000;
pthread_mutex_t sum_mutex;

void* T_sum(void* arg) {
    long local_sum = 0;
    long my_N = N / (long)arg;

    for (long i = 0; i < my_N; i++) {
        local_sum++;
    }

    pthread_mutex_lock(&sum_mutex);
    sum += local_sum;
    pthread_mutex_unlock(&sum_mutex);

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&sum_mutex, NULL);

    int T = argc > 1 ? atoi(argv[1]) : 2;
    pthread_t threads[T];

    for (int i = 0; i < T; i++) {
        pthread_create(&threads[i], NULL, T_sum, (void*)(long)T);
    }

    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("sum = %ld\n", sum);

    pthread_mutex_destroy(&sum_mutex);
    return 0;
}
