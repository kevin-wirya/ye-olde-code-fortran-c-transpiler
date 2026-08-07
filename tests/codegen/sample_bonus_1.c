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
    char A[11];
    A[0] = '\0';
    char B[6];
    B[0] = '\0';
    F77_STR_ASSIGN(A, "HELLO", 10);
    F77_STR_ASSIGN(B, "WORLD", 5);
    printf("%s %s\n", A, B);
    return 0;
}
