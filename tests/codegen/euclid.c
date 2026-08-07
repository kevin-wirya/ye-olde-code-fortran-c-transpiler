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

int NGCD(int* NA, int* NB);

int main(void){
    int na;
    int nb;
    // line 2
    printf("%s\n", "A?");
    // line 3
    scanf("%d", &na);
    // line 4
    if((na <= 0)){
    // line 5
    printf("%s\n", "A must be a positive integer.");
    // line 6
    return 0;
    }
    // line 8
    printf("%s\n", "B?");
    // line 9
    scanf("%d", &nb);
    // line 10
    if((nb <= 0)){
    // line 11
    printf("%s\n", "B must be a positive integer.");
    // line 12
    return 0;
    }
    // line 14
    printf("%s %d %s %d %s %d %s\n", "The GCD of", na, " and", nb, " is", NGCD(&na, &nb), ".");
    // line 15
    return 0;
    return 0;
}

int NGCD(int* NA, int* NB){
    int NGCD_val;
    int ia;
    int ib;
    int itemp;
    // line 19
    ia = (*NA);
    // line 20
    ib = (*NB);
label_1:;
    // line 21
    if((ib != 0)){
    // line 22
    itemp = ia;
    // line 23
    ia = ib;
    // line 24
    ib = (itemp % ib);
    // line 25
    goto label_1;
    }
    // line 27
    NGCD_val = ia;
    // line 28
    return NGCD_val;
    return NGCD_val;
}

