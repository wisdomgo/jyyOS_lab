#include <stdio.h>
#include <pthread.h> // 包含pthread库
#include <unistd.h> // 包含usleep函数

unsigned long balance = 100;

void* Alipay_withdraw(void *amt_ptr) {
    int amt = *(int*)amt_ptr;
    if (balance >= amt) {
        //usleep(5); // 可以根据需要取消注释来模拟延迟

        balance -= amt;
    }
    return NULL;
}

void create_and_join_thread(void* (*start_routine) (void *), void *arg) {
    pthread_t thread; // 定义线程的标识符
    int status;

    // 创建一个新线程
    status = pthread_create(&thread, NULL, start_routine, arg);
    if (status != 0) {
        printf("Error creating thread\n");
        return;
    }

    // 等待线程结束
    pthread_join(thread, NULL);
}

int main() {
    int amt = 100; // 将提取金额定义为变量，以便传递给线程函数

    create_and_join_thread(Alipay_withdraw, &amt);
    create_and_join_thread(Alipay_withdraw, &amt);

    printf("balance = %lu\n", balance);

    return 0;
}
