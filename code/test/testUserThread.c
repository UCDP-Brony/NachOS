#include "syscall.h"
void testFunction(void * arg){
	char * str = (char *)arg;
	SynchPutString(str);
	UserThreadExit();
}

int main(){
	UserThreadCreate(testFunction, (void *)"Hello World !\n");
	return 0;
}