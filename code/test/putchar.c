#include "syscall.h"
void print(char c, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		PutChar(c+i);
	}
	
	PutChar('\n');
	SynchPutString("C'est un succès ! \n");
	char test[5];
	SynchPutString("Test GetChar ! \n");
	char d;
	d = SynchGetChar();
	PutChar(d);
	SynchPutString("Test GetString ! \n");
	SynchGetString(test,5);
	SynchPutString(test);
	SynchPutString("Test des int ! \n");
	int j = 0;
	SynchGetInt(&j);
	SynchPutInt(j);
}
int main()
{
	print('a',4);
	#ifdef CHANGED
	return 0;
	#else
	Halt();
	#endif //CHANGED
}
