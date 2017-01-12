#include "syscall.h"
void testFunction(void * arg){
	int index = (int)arg;
	char str[5] = "";
	char letter = 'a';
	int i;
	//SynchPutInt(index);
	for(i = index; i < index+5; i++){
		str[i - index] = letter+i;
		//SynchPutString("In loop testFunction\n");
	}
	//SynchPutString("After loop testFunction\n");
	SynchPutString(str);
	UserThreadExit();
}

int main(){
	int i;
	for(i = 0; i < 100000; i++){
		UserThreadCreate(testFunction, (void *)0);
		UserThreadCreate(testFunction, (void *)5);
		UserThreadCreate(testFunction, (void *)10);
		UserThreadCreate(testFunction, (void *)15);
		UserThreadCreate(testFunction, (void *)20);
	}
		// UserThreadJoin();
	Halt();
}