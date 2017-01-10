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

	//printf("args = %i\n", params->arg);

	machine->WriteRegister (PCReg, params->function);
	machine->WriteRegister (NextPCReg, params->function + 4);
	machine->WriteRegister (4, params->arg);
	machine->WriteRegister (StackReg, spr - (PageSize*2));
	machine->WriteRegister (RetAddrReg, (int)do_UserThreadExit);
	machine->Run();
}

int do_UserThreadCreate(int f, int arg){
	ThreadArgs args;
	args.function = f;
	args.arg = arg;
	Thread *thread = new Thread("thread utilisateur");
	//printf("DEBUG 1\n");
	thread->Fork(StartUserThread, (int)&args);
	//currentThread->Yield();
	//interrupt->SetLevel(IntOff);
	//currentThread->Sleep();
	//printf("DEBUG 5\n");

	return 0;
}

void do_UserThreadJoin(){
	while(scheduler->FindNextToRun() != NULL){
		currentThread->Yield();
	}
}

void do_UserThreadExit(){
	currentThread->Finish();
}