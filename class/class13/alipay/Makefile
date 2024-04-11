all: alipay

CFLAGS := -O1 -g -fsanitize=thread

alipay: alipay.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f alipay
