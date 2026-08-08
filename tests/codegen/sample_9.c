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

struct globals_t {
    int g1;
    int g2;
} globals;

void PRTGLB();

int main(void){
    // line 7
    globals.g1 = 10;
    // line 8
    globals.g2 = 20;
    // line 9
    PRTGLB();
    return 0;
}

void PRTGLB(){
    // line 16
    printf("%d %d\n", globals.g1, globals.g2);
    // line 17
    return;
}

