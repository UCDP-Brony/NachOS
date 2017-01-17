#include "copyright.h"
#include "userthread.h"
#include "syscall.h"
#include "system.h"
#include "synch.h"
#include "addrspace.h"

typedef struct ThreadArgs{
	int function;
	int arg;
}ThreadArgs;

Semaphore *verifying = new Semaphore("Verifying Thread", 1);
Semaphore *starting = new Semaphore("Starting Thread", 1);

static void StartUserThread(int f){

	starting->P();

	DEBUG('a', "Before InitRegisters");

	currentThread->space->InitRegisters();

	DEBUG('a', "Before RestoreRegister");

	currentThread->space->RestoreState();

	ThreadArgs * params = (ThreadArgs *)f;

	int spr = machine->ReadRegister (StackReg);
	machine->WriteRegister (StackReg, spr - ((PageSize*2)*compteur));

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
	verifying->P();

	ThreadArgs *args = new ThreadArgs();
	args->function = f;
	args->arg = arg;
	Thread *thread = new Thread("thread utilisateur");

	if(currentThread->space->addThreadToList((void*)thread)){
		thread->Fork(StartUserThread, (int)args);
		currentThread->Yield();
	} else {
		thread = -1;
	}
	verifying->V();	

	return (int)thread;
}

void do_UserThreadJoin(unsigned int address){
	ThreadCond *tc = currentThread->space->findThreadInList((void*) address);
	if(tc != (void*)-1){
		tc->mutex->Acquire();
		if(address == (unsigned int)tc->thread){
			tc->cond->Wait(tc->mutex);
		}
		tc->mutex->Release();
	}
}

void do_UserThreadExit(){
	ThreadCond *tc = currentThread->space->findThreadInList(currentThread);
	tc->cond->Broadcast(tc->mutex);
	currentThread->space->removeThreadFromList(currentThread);
	delete currentThread;
}