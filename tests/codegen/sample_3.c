#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    int MATRIX[9];
    int I, J;
    for(I=1; I<=3; I+=1){
    for(J=1; J<=3; J+=1){
    MATRIX[(((I)-1)+((J)-1)*3)] = (I * J);
label_20:;
    }
label_10:;
    }
    printf("%s\n", "Matrix init done");
    return 0;
}
