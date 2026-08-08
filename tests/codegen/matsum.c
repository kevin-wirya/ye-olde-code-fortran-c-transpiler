#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    // line 3
    int a[6];
    // line 4
    int i, j, total;
    // line 6
    a[(((1)-1)+((1)-1)*3)] = 1;
    // line 7
    a[(((2)-1)+((1)-1)*3)] = 2;
    // line 8
    a[(((3)-1)+((1)-1)*3)] = 3;
    // line 9
    a[(((1)-1)+((2)-1)*3)] = 4;
    // line 10
    a[(((2)-1)+((2)-1)*3)] = 5;
    // line 11
    a[(((3)-1)+((2)-1)*3)] = 6;
    // line 15
    total = 0;
    // line 16
    for(i=1; i<=3; i+=1){
    // line 17
    for(j=1; j<=2; j+=1){
    // line 18
    total = (total + a[(((i)-1)+((j)-1)*3)]);
    }
    }
    // line 21
    printf("%d\n", total);
    return 0;
}

