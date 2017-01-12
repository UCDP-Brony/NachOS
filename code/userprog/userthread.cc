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

	printf("Compteur : %i\n", compteur);
	ThreadArgs * params = (ThreadArgs *)f;
	printf("Address contained in f : %x\n", f);
	printf("Address in arg : %i\n", params->function);

	int spr = machine->ReadRegister (StackReg);
	machine->WriteRegister (StackReg, spr - ((PageSize*2)*compteur));

	printf("After param declaration\n");
	// int* pvalue = (int*)params->arg;
	// printf("args = %x\n", *pvalue);

	DEBUG('a', "Before WriteRegister");
	printf("Before WriteRegister\n");
	machine->WriteRegister (RetAddrReg, PCReg+24);
	machine->WriteRegister (PCReg, params->function);
	machine->WriteRegister (NextPCReg, params->function + 4);
	machine->WriteRegister (4, params->arg);
	DEBUG('a',"Before Run");
	printf("Before Run\n");
	starting->V();
	machine->Run();
}

int do_UserThreadCreate(int f, int arg){
	ThreadArgs *args = new ThreadArgs();
	args->function = f;
	args->arg = arg;
	printf("Address of args : %p\n", &args);
	printf("Argument : %i\n", args->function);
	Thread *thread = new Thread("thread utilisateur");
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