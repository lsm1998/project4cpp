#include <stdio.h>

void hello(char* name) {
    printf("hello %s \n", name);
}

int main()
{
    int i;
    i = 0;
    while (i <= 10) {
        hello("lsm");
        i++;
    }
    return 0;
}