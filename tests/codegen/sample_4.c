#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    bool FLAG, RESULT;
    int A, B;
    A = 5;
    B = 10;
    FLAG = true;
    if(((A < B) && FLAG)){
    RESULT = .FALSE.false;
    printf("%s\n", "A is less than B");
    } else {
    RESULT = false;
    printf("%s\n", "A is >= B");
    }
    return 0;
}
