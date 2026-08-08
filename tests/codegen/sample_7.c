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

float CALC(float* VAL);

int main(void){
    // line 5
    float x, y, calc;
    // line 6
    x = 5.0;
    // line 7
    y = CALC(&x);
    // line 8
    printf("%f\n", y);
    return 0;
}

float CALC(float* VAL){
    float CALC_val;
    // line 14
    CALC_val = pow((*VAL), 2.0);
    // line 15
    return CALC_val;
    return CALC_val;
}

