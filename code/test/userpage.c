#include "syscall.h"

int main(){
	SynchPutString("Before userpage0\n");
    ForkExec("./userpage0");
	SynchPutString("Before userpage1\n");
    ForkExec("./userpage1");
    return 0;
}