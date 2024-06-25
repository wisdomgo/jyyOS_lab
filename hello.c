
#include<stdarg.h>

void print(const char* s)
{
    va_list ap;
    va_start(ap,s);
    while(s)
    {
        for(;*s;s++)
            putch(*s);
        s = va_arg(ap,const char*);
    }
    va_end(ap);
}