#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int myfunc(int* val);

int main(void){
    // line 5
    int res, n;
    // line 6
    n = 4;
    // line 7
    res = myfunc(&n);
    // line 8
    printf("%d\n", res);
    return 0;
}

int myfunc(int* val){
    int myfunc_val;
    // line 14
    myfunc_val = ((*val) * 3);
    // line 15
    return myfunc_val;
}

