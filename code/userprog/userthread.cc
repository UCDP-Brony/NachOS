#include "copyright.h"
#include "userthread.h"
#include "syscall.h"
#include "system.h"
#include "synch.h"
#include "addrspace.h"
#include "threadargs.h"


Semaphore *verifying = new Semaphore("Verifying Thread", 1);
Semaphore *starting = new Semaphore("Starting Thread", 1);

//----------------------------------------------------------------------
// StartUserThread
//		Initialize the currentThread then run it.
//		
//		Load the registers for the thread with a return address, the address of the function to execute
//		and the address of the arguments of the function.
//		Also change the stack pointer so it is at the right position.
//
//		Then it runs the thread.
//
//		"f" is the address of the structure containing the arguments for the thread (the function to execute
//		and its arguments).
//----------------------------------------------------------------------

static void StartUserThread(int f){

	starting->P();

	DEBUG('t', "Before InitRegisters");

	currentThread->space->InitRegisters();

	DEBUG('t', "Before RestoreRegister");

	currentThread->space->RestoreState();

	ThreadArgs * params = (ThreadArgs *)f;

	int spr = machine->ReadRegister (StackReg);
	machine->WriteRegister (StackReg, spr - ((PageSize*2)*currentThread->space->getThreadID(currentThread)));

	DEBUG('t', "Before WriteRegister");

	machine->WriteRegister (RetAddrReg, PCReg+24);
	machine->WriteRegister (PCReg, params->function);
	machine->WriteRegister (NextPCReg, params->function + 4);
	machine->WriteRegister (4, params->arg);

	DEBUG('t',"Before Run");

	starting->V();
	machine->Run();
}

//----------------------------------------------------------------------
// do_UserThreadCreate
//		Create the Thread to run.
//
//		Create a new Thread and add it to the list of thread of the current address space.
//		If there is enough space in the address space then the thread calls the Fork function
//		with StartUserThread as the function to execute.
//		Return the address of the Thread if successful, -1 otherwise.
//		
//		"f" is the address og the function that the thread have to execute and "arg" is the
//		argument of that function.
//----------------------------------------------------------------------

int do_UserThreadCreate(int f, int arg){
	verifying->P();

	ThreadArgs *args = new ThreadArgs();
	args->function = f;
	args->arg = arg;
	Thread *thread = new Thread("thread utilisateur");

	if(currentThread->space->addThreadToList((void*)thread)){
		DEBUG('t', "Thread %p starting\n", thread);
		thread->Fork(StartUserThread, (int)args);
		currentThread->Yield();
	} else {
		thread = (Thread*)-1;
	}
	verifying->V();
	return (int)thread;
}

//----------------------------------------------------------------------
// do_UserThreadJoin
//		Wait for the thread identified by "address" to finish.
//
//		Search the thread in the list of threads and take its Lock.
//		Then the currentThread adds himself to the Condition of the thread.
//		
//		"address" is the address of the thread to wait.
//----------------------------------------------------------------------

void do_UserThreadJoin(unsigned int address){
	ThreadCond *tc = currentThread->space->findThreadInList((void*) address);
	if(tc != (void*)-1){
		tc->mutex->Acquire();
		if(address == (unsigned int)tc->thread){
			tc->cond->Wait(tc->mutex);
			DEBUG('t',"After wait\n");
		}
		tc->mutex->Release();
	}
}

//----------------------------------------------------------------------
// do_UserThreadExit
//		Finish currentThread.
//
//		Search the thread in the list and do a broadcast on the Condition to release every
//		waiting threads.
//		Then remove the thread from the list and finish it.
//----------------------------------------------------------------------

void do_UserThreadExit(){
	ThreadCond *tc = currentThread->space->findThreadInList(currentThread);
	if(tc != (void *)-1){
		tc->cond->Broadcast(tc->mutex);
		DEBUG('t',"Thread %p exiting\n", currentThread);
		currentThread->space->removeThreadFromList(currentThread);
		currentThread->Finish();
	}
}