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
    // line 3
    char a[11];
    a[0] = '\0';
    // line 4
    char b[6];
    b[0] = '\0';
    // line 5
    F77_STR_ASSIGN(a, "HELLO", 10);
    // line 6
    F77_STR_ASSIGN(b, "WORLD", 5);
    // line 7
    printf("%s %s\n", a, b);
    return 0;
}

