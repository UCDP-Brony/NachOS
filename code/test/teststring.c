#include "syscall.h"

int main(){
	SynchPutString("Test des io relatifs aux string \n");
	char test[255];
	SynchGetString(test,255);
	SynchPutString(test);
	return 0;
}