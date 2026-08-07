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

struct acc_t {
    int runtotal;
    int callcnt;
} acc;

void ADDSQUARE(int* VAL);

int main(void){
    // line 3
    int n, i;
    // line 8
    acc.runtotal = 0;
    // line 9
    acc.callcnt = 0;
    // line 10
    n = 5;
    // line 12
    for(i=1; i<=n; i+=1){
    // line 13
    ADDSQUARE(&i);
label_10:;
    }
    // line 16
    printf("%d\n", acc.runtotal);
    // line 17
    printf("%d\n", acc.callcnt);
    return 0;
}

void ADDSQUARE(int* VAL){
    // line 26
    int total, cnt;
    // line 28
    total = (total + ((*VAL) * (*VAL)));
    // line 29
    cnt = (cnt + 1);
    // line 31
    return;
}

