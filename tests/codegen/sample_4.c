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

