#include "syscall.h"
void print(char c, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		PutChar(c+i);
	}
	
	PutChar('\n');
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