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

int main(void){
    // line 5
    int matrix[9];
    // line 6
    int i, j;
    // line 7
    for(i=1; i<=3; i+=1){
    // line 8
    for(j=1; j<=3; j+=1){
    // line 9
    matrix[(((i)-1)+((j)-1)*3)] = (i * j);
    }
    }
    // line 12
    printf("%s\n", "Matrix init done");
    return 0;
}

