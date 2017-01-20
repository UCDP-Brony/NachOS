#include "syscall.h"
void testFunction(void * arg){
	int index = (int)arg;
	char letter = 'a';
	int i;
	for(i = index; i < index+12; i++){
		PutChar(letter+i);
	}
	UserThreadExit();
}

int main(){
	SynchPutString("Entering Main\n");
	int id = UserThreadCreate(testFunction, (void *)0);
	int id2 = UserThreadCreate(testFunction, (void *)12);
	UserThreadJoin(id);
	UserThreadJoin(id2);
	//SynchPutString("\n");
	return 0;
}