#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    // line 5
    float x, y, z;
    // line 6
    x = 2.5E1;
    // line 7
    y = 3.0D-1;
    // line 8
    z = ((pow(x, 2) + ((y * 5.0) / 2.0)) - 1.0);
    // line 9
    printf("%f\n", z);
    // line 10
    goto label_99;
    // line 11
    printf("%s\n", "This is skipped");
label_99:;
    return 0;
}

