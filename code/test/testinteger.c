#include "syscall.h"

int main(){
	SynchPutString("Test des io relatifs aux int \n");
	int j = 42;
	SynchGetInt(&j);
	SynchPutInt(j);
	SynchPutString("\n");
	return 0;
}