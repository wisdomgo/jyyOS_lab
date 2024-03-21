CFLAGS := -O2 -I$(TLIB_PATH)

all: atomic mutex spin

atomic:
	gcc $(CFLAGS) -o atomic sum-atomic.c main.c -lpthread

mutex:
	gcc $(CFLAGS) -o mutex sum-mutex.c main.c -lpthread

spin:
	gcc $(CFLAGS) -o spin sum-spin.c main.c -lpthread

clean:
	rm -f atomic mutex spin
