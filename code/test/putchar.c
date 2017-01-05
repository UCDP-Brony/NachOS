#include "syscall.h"
void print(char c, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		PutChar(c+i);
	}
	
	PutChar('\n');
	SynchPutString("C'est un succès ! \n");
	SynchPutString("Test GetChar, deux entrées, puis on les affiche ! \n");
	char d;
	char d2;
	d = SynchGetChar();
	d2 = SynchGetChar();
	PutChar(d);
	PutChar(d2);
	SynchPutString("Test GetChar ! \n");
	d = SynchGetChar();
	PutChar(d);
	SynchPutString("Test GetChar ! \n");
	d = SynchGetChar();
	PutChar(d);
	SynchPutString("Test GetString ! \n");
	char test[5];
	//char unmot[] = {'t','e','s','t','\0'};
	SynchGetString(test,12);
	//SynchPutString(unmot);
	SynchPutString(test);
	SynchPutString("Test des int ! \n");
	int j = 42;
	SynchGetInt(&j);
	SynchPutInt(j);
	SynchPutString("Test des int ! \n");
	SynchGetInt(&j);
	SynchPutInt(j);
	SynchPutString("Test des int ! \n");
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
