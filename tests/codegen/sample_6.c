#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define F77_STR_ASSIGN(dest, src, len) \
    do { \
        strncpy(dest, src, len); \
        int _l = strlen(src); \
        for(int _i = _l; _i < len; _i++) dest[_i] = ' '; \
        dest[len] = '\0'; \
    } while(0)

void fill(int* a, int* sz);

int main(void){
    // line 5
    int arr[5];
    // line 6
    int n;
    // line 7
    n = 5;
    // line 8
    fill(arr, &n);
    // line 9
    printf("%d\n", arr[(1 - 1)]);
    return 0;
}

void fill(int* a, int* sz){
    // line 14
    int sz, i;
    // line 15
    int a[10];
    // line 16
    for(i=1; i<=(*sz); i+=1){
    // line 17
    a[(i - 1)] = (i * 10);
    }
    // line 19
    return;
}

