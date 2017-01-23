#include "syscall.h"

int main(){
    ForkExec("./matmult");
    ForkExec("./matmult");
    return 0;
}