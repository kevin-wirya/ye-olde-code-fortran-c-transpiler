#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    // line 5
    bool flag, result;
    // line 6
    int a, b;
    // line 7
    a = 5;
    // line 8
    b = 10;
    // line 9
    flag = true;
    // line 10
    if(((a < b) && flag)){
    // line 11
    result = !false;
    // line 12
    printf("%s\n", "A is less than B");
    } else {
    // line 14
    result = false;
    // line 15
    printf("%s\n", "A is >= B");
    }
    return 0;
}

