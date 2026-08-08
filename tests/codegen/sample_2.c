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
    printf("%s\n", "Hello, World!");
    return 0;
}

