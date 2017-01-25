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

	int id = UserThreadCreate(testFunction, (void *)0);
	int id2 = UserThreadCreate(testFunction, (void *)5);
	
	UserThreadJoin(id);
	UserThreadJoin(id2);
	return 0;
}