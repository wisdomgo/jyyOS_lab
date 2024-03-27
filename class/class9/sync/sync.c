#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // Include this header for intptr_t

enum {
    C = 40,
    D = 42,
    E = 44,
    F = 45,
    G = 47,
    A = 49,
    B = 51,
    C0 = C - 12,
    D0 = D - 12,
    E0 = E - 12,
    F0 = F - 12,
    G0 = G - 12,
    A0 = A - 12,
    B0 = B - 12,
};

pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;

int n = -1;

void wait_next_beat(int expect) {
    // This is a spin-wait loop.
retry:
    pthread_mutex_lock(&lk);
    // This read is protected by a mutex.
    int got = n;
    pthread_mutex_unlock(&lk);

    if (got != expect) goto retry;
}

void release_beat() {
    pthread_mutex_lock(&lk);
    n++;
    pthread_mutex_unlock(&lk);
}

int NOTES[3][8] = {
    {C, G0, A0, E0, F0, C0, F0, G0},
    {E, B0, C0, G0, A0, E0, A0, B0},
    {G, D,  E,  B0, C0, G0, C0, D0},
};

void *T_player(void *arg) {
    int id = (int)(intptr_t)arg; // Safe conversion from pointer to integer
    int *notes, expect = 0;
    char cmd[128];

    switch (id) {
        case 1: notes = NOTES[0]; break;
        case 2: notes = NOTES[1]; break;
        case 3: notes = NOTES[2]; break;
    }

    while (1) {
        wait_next_beat(expect);

        int note = notes[expect % 8];
        sprintf(cmd,
            "ffplay -nodisp -autoexit"
            " -loglevel quiet"
            " piano_key_%d.wav"
            " > /dev/null &", note);

        system(cmd);

        expect++;
    }
    return NULL; // This line is required to match the expected signature.
}

void *T_conductor(void *arg) {
    char buf[32];
    while (1) {
        printf("> ");
        fgets(buf, sizeof(buf), stdin);
        release_beat();
    }
    return NULL;
}

int main() {
    pthread_t threads[4];

    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, T_player, (void *)(intptr_t)(i + 1));
    }
    pthread_create(&threads[3], NULL, T_conductor, NULL);

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
