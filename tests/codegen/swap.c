#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define F77_STR_ASSIGN(dest, src, len) \
    do { \
        strncpy(dest, src, len); \
        int _l = strlen(src); \
        for(int _i = _l; _i < len; _i++) dest[_i] = ' '; \
        dest[len] = '\0'; \
    } while(0)

void ORDER(int* A, int* B, bool* FLAG);

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
    ORDER(&x, &y, &swapped);
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

void ORDER(int* A, int* B, bool* FLAG){
    // line 26
    int temp;
    // line 28
    if(((*A) > (*B))){
    // line 29
    temp = (*A);
    // line 30
    (*A) = (*B);
    // line 31
    (*B) = temp;
    // line 32
    (*FLAG) = true;
    } else {
    // line 34
    (*FLAG) = false;
    }
    // line 37
    return;
}

