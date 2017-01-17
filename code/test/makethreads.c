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
	//UserThreadExit();
}

int main(){
	SynchPutString("Test ! \n");

	int id = UserThreadCreate(testFunction, (void *)0);
	int id2 = UserThreadCreate(testFunction, (void *)5);
	UserThreadCreate(testFunction, (void *)10);
	UserThreadCreate(testFunction, (void *)15);
	UserThreadCreate(testFunction, (void *)20);
	SynchPutString("wait id : ");
	SynchPutInt(id);
	SynchPutString("\n");
	//UserThreadJoin(id);

	SynchPutString("wait id : ");
	SynchPutInt(id2);
	SynchPutString("\n");
	//UserThreadJoin(id2);
	return 0;
}