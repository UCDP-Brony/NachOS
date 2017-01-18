#include "syscall.h"

int main(){
	sem_t monSem = SemCreate(1);
	SemP(monSem);
	//SemP(monSem0);
	SemV(monSem);
	SemDestroy(monSem);
	
	return 0;
}