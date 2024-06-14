#include <stdlib.h>
#include <stdio.h>
#include "arvores.h"


int main (void){
    benchmark_avl(10000);
    printf("\n");
    benchmark_rb(10000);
}
