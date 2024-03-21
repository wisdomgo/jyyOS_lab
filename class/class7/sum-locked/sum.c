#include <pthread.h>
#include<stdio.h>
#include<stdint.h>
#define T        3
#define N  1000000

#define LOCKED   1
#define UNLOCKED 0

int status = UNLOCKED;

void lock() {
    int expected;
    do {
        // Try compare status with expected.
        // If the comparison succeeded, perform
        // an exchange.
        expected = UNLOCKED;
        asm volatile (
            "lock cmpxchgl %2, %1"
            : "+a" (expected) // Value for comparison.
                              // x86 uses eax/rax.
            : "m" (status),   // Memory location.
              "r" (LOCKED)    // Value to be written if
                              // status == expected
            : "memory", "cc"
        );
    } while (expected != UNLOCKED);
}

void unlock() {
    // To be safer:
    //
    // asm volatile (
    //     "movl %1, %0"
    //     : "=m" (status)
    //     : "r" (UNLOCKED)
    //     : "memory"
    // );

    // But actually we can do this:
    status = UNLOCKED;
}


long volatile sum = 0;

void T_sum(int tid) {
    for (int i = 0; i < N; i++) {
        lock(tid != 1);

        // This critical section is even longer; but
        // it should be safe--the world is stopped.
        // We also marked sum as volatile to make
        // sure it is loaded and stored in each
        // loop iteration.
        for (int _ = 0; _ < 10; _++) {
            sum++;
        }

        unlock(tid != 1);
    }

    printf("Thread %d: sum = %ld\n", tid, sum);
}

int main() {
    pthread_t threads[T];
    for (int i = 0; i < T; i++) {
        if(pthread_create(&threads[i], NULL, (void *(*)(void *))T_sum, (void *)(intptr_t)i)) {
            perror("pthread_create failed");
            return 1;
        }
    }

    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("sum  = %ld\n", sum);
    printf("%d*n = %ld\n", T * 10, T * 10L * N);
}