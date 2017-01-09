#include "copyright.h"
#include "userthread.h"
#include "syscall.h"
#include "system.h"

typedef struct ThreadArgs{
	int function;
	int arg;
}ThreadArgs;

static void StartUserThread(int f){

	currentThread->space->InitRegisters();
	currentThread->space->RestoreState();
	ThreadArgs * params = (ThreadArgs *) f;

	int spr = machine->ReadRegister (StackReg);

	printf("args = %p\n",(void *)params->arg);

	machine->WriteRegister (PCReg, params->function);
	machine->WriteRegister (NextPCReg, params->function + 4);
	machine->WriteRegister (4, params->arg);
	machine->WriteRegister (StackReg, spr - (PageSize*2));
	//machine->WriteRegister (RetAddrReg, UserThreadExit);
	machine->Run();
}

int do_UserThreadCreate(int f, int arg){
	ThreadArgs args;
	args.function = f;
	args.arg = arg;
	Thread *thread = new Thread("thread utilisateur");
	printf("DEBUG 1\n");
	thread->Fork(StartUserThread, (int)&args);
	//currentThread->Yield();
	interrupt->SetLevel(IntOff);
	currentThread->Sleep();
	printf("DEBUG 5\n");

	return 0;
}

void do_UserThreadExit(){
	currentThread->Finish();
}