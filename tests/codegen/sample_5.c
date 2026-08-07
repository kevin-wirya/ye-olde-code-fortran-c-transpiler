#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    float X, Y, Z;
    X = 2.5E1;
    Y = 3.0D-1;
    Z = (((X ** 2) + ((Y * 5.0) / 2.0)) - 1.0);
    printf("%d\n", Z);
    goto label_99;
    printf("%s\n", "This is skipped");
label_99:;
    return 0;
}
