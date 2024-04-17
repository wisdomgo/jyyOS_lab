#include<stdio.h>
int main()
{
    printf("hello\n");
    *((int*)(NULL)) = 0;
}