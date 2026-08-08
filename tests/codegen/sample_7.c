#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

float calc(float* val);

int main(void){
    // line 5
    float x, y;
    // line 6
    x = 5.0;
    // line 7
    y = calc(&x);
    // line 8
    printf("%f\n", y);
    return 0;
}

float calc(float* val){
    float calc_val;
    // line 14
    calc_val = pow((*val), 2.0);
    // line 15
    return calc_val;
}

