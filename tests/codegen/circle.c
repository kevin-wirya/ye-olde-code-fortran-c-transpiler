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

struct stats_t {
    int count;
} stats;

float CALCAREA(float* RADIUS);

int main(void){
    // line 3
    float r, area1, area2, bigger;
    // line 7
    stats.count = 0;
    // line 9
    r = 2.0;
    // line 10
    area1 = CALCAREA(&r);
    // line 11
    printf("%f\n", area1);
    // line 13
    r = 3.0;
    // line 14
    area2 = CALCAREA(&r);
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

float CALCAREA(float* RADIUS){
    float CALCAREA_val;
    // line 26
    int cnt;
    // line 29
    CALCAREA_val = ((3.14159 * (*RADIUS)) * (*RADIUS));
    // line 30
    cnt = (cnt + 1);
    // line 32
    return CALCAREA_val;
    return CALCAREA_val;
}

