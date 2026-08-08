#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void order(int* a, int* b, bool* flag);

int main(void){
    // line 3
    int x, y;
    // line 4
    bool swapped;
    // line 6
    x = 10;
    // line 7
    y = 3;
    // line 8
    swapped = false;
    // line 10
    order(&x, &y, &swapped);
    // line 12
    if(swapped){
    // line 13
    printf("%s\n", "SWAPPED");
    } else {
    // line 15
    printf("%s\n", "NOT SWAPPED");
    }
    // line 18
    printf("%d %d\n", x, y);
    return 0;
}

void order(int* a, int* b, bool* flag){
    // line 26
    int temp;
    // line 28
    if(((*a) > (*b))){
    // line 29
    temp = (*a);
    // line 30
    (*a) = (*b);
    // line 31
    (*b) = temp;
    // line 32
    (*flag) = true;
    } else {
    // line 34
    (*flag) = false;
    }
    // line 37
    return;
}

