#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct GLOBALS_t {
    int G1;
    int G2;
} GLOBALS;

int main(void){
    int G1, G2;
    G1 = 10;
    G2 = 20;
    PRTGLB();
void PRTGLB(){
    int G1, G2;
    printf("%d\n", G1, "%d\n", G2);
    return;
}

    return 0;
}
