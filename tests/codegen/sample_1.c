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
    int n, i;
    // line 6
    n = 5;
    // line 7
    for(i=1; i<=n; i+=1){
    // line 8
    printf("%d\n", i);
    }
    return 0;
}

