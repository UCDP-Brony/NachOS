#include "copyright.h"
#include "userthread.h"
#include "syscall.h"
#include "system.h"
#include "synch.h"

typedef struct ThreadArgs{
	int function;
	int arg;
}ThreadArgs;

int compteur = 0;
Semaphore *starting = new Semaphore("Starting Thread", 1);

static void StartUserThread(int f){

	starting->P();
	compteur++;
	DEBUG('a', "Before InitRegisters");
	currentThread->space->InitRegisters();
	DEBUG('a', "Before RestoreRegister");
	currentThread->space->RestoreState();

	ThreadArgs * params = (ThreadArgs *)f;

	int spr = machine->ReadRegister (StackReg);
	machine->WriteRegister (StackReg, spr - ((PageSize*2)*compteur));

	// int* pvalue = (int*)params->arg;
	// printf("args = %x\n", *pvalue);

	DEBUG('a', "Before WriteRegister");
	machine->WriteRegister (RetAddrReg, PCReg+24);
	machine->WriteRegister (PCReg, params->function);
	machine->WriteRegister (NextPCReg, params->function + 4);
	machine->WriteRegister (4, params->arg);
	DEBUG('a',"Before Run");
	starting->V();
	machine->Run();
}

int do_UserThreadCreate(int f, int arg){
	ThreadArgs *args = new ThreadArgs();
	args->function = f;
	args->arg = arg;
	Thread *thread = new Thread("thread utilisateur");
	printf("Thread : %p\n", thread);
	//printf("DEBUG 1\n");
	thread->Fork(StartUserThread, (int)args);
	//currentThread->Yield();
	//interrupt->SetLevel(IntOff);
	//currentThread->Sleep(); // if this is not commented then the first thread do what it is supposed to do, otherwise the adresses used later are corrupted
	//printf("DEBUG 5\n");

	return 0;
}

void do_UserThreadJoin(){
	/*while(scheduler->FindNextToRun() != NULL){
		currentThread->Yield();
	}*/
	interrupt->SetLevel(IntOff);
	currentThread->Sleep();
}

void do_UserThreadExit(){
	currentThread->Finish();
}