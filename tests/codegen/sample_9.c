#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct globals_t {
    int g1;
    int g2;
} globals;

void prtglb();

int main(void){
    // line 7
    globals.g1 = 10;
    // line 8
    globals.g2 = 20;
    // line 9
    prtglb();
    return 0;
}

void prtglb(){
    // line 16
    printf("%d %d\n", globals.g1, globals.g2);
    // line 17
    return;
}

