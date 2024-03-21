#include<stdio.h>
#include<stdlib.h>
//#include<common.h>

volatile int counter = 0;
int loops;

void* worker(void* arg)
{
    for(int i = 0;i < loops;i++)
        counter++;
    return NULL;
}

int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        fprintf("error");
        exit(1);
    }
    loops = atoi(argv[1]);
    pthread_t p1,p2;
    printf("%d\n",counter);

    pthread_create(&p1,NULL,worker,NULL);
    pthread_create(&p2,NULL,worker,NULL);
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    printf("%d\n",counter);
    return 0;
}