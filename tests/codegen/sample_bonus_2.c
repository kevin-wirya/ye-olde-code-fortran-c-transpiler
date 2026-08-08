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
    int i;
    // line 4
    i = 2;
    // line 5
    switch(i){
        case 1: goto label_10; break;
        case 2: goto label_20; break;
        case 3: goto label_30; break;
    }
label_10:;
    // line 7
    printf("%d\n", 100);
    // line 8
    goto label_40;
label_20:;
    // line 10
    printf("%d\n", 200);
    // line 11
    goto label_40;
label_30:;
    // line 13
    printf("%d\n", 300);
label_40:;
    return 0;
}

