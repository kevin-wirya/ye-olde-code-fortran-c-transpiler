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

struct state_t {
    bool done;
} state;

int main(void){
    // line 5
    int n, i, sum;
    // line 6
    float x[10];
    // line 10
    scanf("%d", &n);
    // line 11
    sum = 0;
    // line 12
    state.done = false;
    // line 14
    for(i=1; i<=n; i+=1){
    // line 15
    x[(i - 1)] = (1.0E0 * i);
    // line 16
    sum = (sum + i);
    // line 17
    if((sum >= 100)){
    // line 18
    state.done = true;
    // line 19
    goto label_999;
    }
label_50:;
    }
label_999:;
    // line 24
    printf("%s %d %s %d\n", "SUM:", sum, "DONE:", state.done);
    return 0;
}

