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

int MYFUNC(int* VAL);

int main(void){
    // line 5
    int res, myfunc, n;
    // line 6
    n = 4;
    // line 7
    res = MYFUNC(&n);
    // line 8
    printf("%d\n", res);
    return 0;
}

int MYFUNC(int* VAL){
    int MYFUNC_val;
    // line 13
    int myfunc;
    // line 14
    MYFUNC_val = ((*VAL) * 3);
    // line 15
    return MYFUNC_val;
    return MYFUNC_val;
}

