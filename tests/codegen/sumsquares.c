#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct acc_t {
    int runtotal;
    int callcnt;
} acc;

void addsquare(int* val);

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
    addsquare(&i);
    }
    // line 16
    printf("%d\n", acc.runtotal);
    // line 17
    printf("%d\n", acc.callcnt);
    return 0;
}

void addsquare(int* val){
    // line 28
    acc.runtotal = (acc.runtotal + ((*val) * (*val)));
    // line 29
    acc.callcnt = (acc.callcnt + 1);
    // line 31
    return;
}

