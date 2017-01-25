#include "syscall.h"
void testFunction(void * arg){
	int index = (int)arg;
	char letter = 'a';
	int i;
	for(i = index; i < index+5; i++){
		PutChar(letter+i);
	}
	UserThreadExit();
}

int main(){

	UserThreadCreate(testFunction, (void *)0);
	UserThreadCreate(testFunction, (void *)5);

	return 0;
}