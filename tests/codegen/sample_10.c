#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct STATE_t {
    bool DONE;
} STATE;

int main(void){
    int N, I, SUM;
    float X[10];
    bool DONE;
    scanf("%d", &N);
    SUM = 0;
    DONE = .FALSE.;
    for(I=1; I<=N; ++I){
    X[I - 1] = (1.0E0 * I);
    SUM = (SUM + I);
    if((SUM >= 100)){
    DONE = .TRUE.;
    goto label_999;
    }
label_50:;
    }
label_999:;
    printf("%d\n", 'SUM:', "%d\n", SUM, "%d\n", 'DONE:', "%d\n", DONE);
    return 0;
}
