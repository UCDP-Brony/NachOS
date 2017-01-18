#include "syscall.h"
void testFunction(void * arg){
	char * str = (char *)arg;
	SynchPutString(str);
	UserThreadExit();
}

int main(){
	int id = UserThreadCreate(testFunction, (void *)"Hello World !\n");
    UserThreadJoin(id);
	return 0;
}