#include "syscall.h"

int main(){
	SynchPutString("Test des io relatifs aux char \n");
	char d;
	d = SynchGetChar();
	PutChar(d);
	
	return 0;
}