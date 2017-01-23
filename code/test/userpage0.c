#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

const int N = 25;

void puts(char *s){
	char *p; for (p = s; *p != '\0'; p++) PutChar(*p);
}

void f(void *s){
	int i; for(i=0; i < N; i++) puts((char *)s);
	UserThreadExit();
}

int main(){
	int id = UserThreadCreate(f, (void  *) THIS);
	//SynchPutInt(id);
	f((void *)THAT);
	UserThreadJoin(id);
	return 0;
}