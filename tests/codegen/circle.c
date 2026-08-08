#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct stats_t {
    int count;
} stats;

float calcarea(float* radius);

int main(void){
    // line 3
    float r, area1, area2, bigger;
    // line 7
    stats.count = 0;
    // line 9
    r = 2.0;
    // line 10
    area1 = calcarea(&r);
    // line 11
    printf("%f\n", area1);
    // line 13
    r = 3.0;
    // line 14
    area2 = calcarea(&r);
    // line 15
    printf("%f\n", area2);
    // line 17
    bigger = ((area1 > area2) ? (area1) : (area2));
    // line 18
    printf("%f\n", bigger);
    // line 19
    printf("%d\n", stats.count);
    return 0;
}

float calcarea(float* radius){
    float calcarea_val;
    // line 29
    calcarea_val = ((3.14159 * (*radius)) * (*radius));
    // line 30
    stats.count = (stats.count + 1);
    // line 32
    return calcarea_val;
}

