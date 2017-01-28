/*Install gcc compiler and openmp support by homebrew:*/
/*
alias gcc=gcc-6
alias clang=gcc-6
alias g++=g++-6
*/

#include <stdio.h>
#include <omp.h>

int  print(int i){
    int tmp = 0;
    for(int i=0;i<100000000;i++){
        tmp += 1; // time consuming loop
    }

    printf("%d\t",i);
    return i;
}

int main(){

    #pragma omp parallel for
    for(int i=0; i<10;i++){
       print(i);
    }
    return 0;
}
