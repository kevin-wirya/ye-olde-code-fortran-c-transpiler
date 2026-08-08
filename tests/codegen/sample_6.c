#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void fill(int* a, int* sz);

int main(void){
    // line 5
    int arr[5];
    // line 6
    int n;
    // line 7
    n = 5;
    // line 8
    fill(arr, &n);
    // line 9
    printf("%d\n", arr[(1 - 1)]);
    return 0;
}

void fill(int* a, int* sz){
    // line 14
    int i;
    // line 15
    int b[10];
    // line 16
    for(i=1; i<=(*sz); i+=1){
    // line 17
    b[(i - 1)] = (i * 10);
    }
    // line 19
    return;
}

