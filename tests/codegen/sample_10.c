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
    DONE = false;
    for(I=1; I<=N; I+=1){
    X[(I - 1)] = (1.0E0 * I);
    SUM = (SUM + I);
    if((SUM >= 100)){
    DONE = true;
    goto label_999;
    }
label_50:;
    }
label_999:;
    printf("%s %d %s %d\n", "SUM:", SUM, "DONE:", DONE);
    return 0;
}
