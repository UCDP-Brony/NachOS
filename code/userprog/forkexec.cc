#include "forkexec.h"
#include "syscall.h"
#include "system.h"
#include "addrspace.h"
#include "threadargs.h"

void execProcess(int filenameAddress){
	printf("File : %x\n", filenameAddress);
	char * fileName = (char *)filenameAddress;
    OpenFile *executable = fileSystem->Open (fileName);
    if (executable == NULL)
    {
      synchConsole->SynchPutString("executable = NULL\n");
      return;
    }

    currentThread->space = new AddrSpace (executable);
    delete executable;		// close file

    currentThread->space->InitRegisters ();	// set the initial register values
    currentThread->space->RestoreState ();	// load page table register
   // scheduler->ReadyToRun(currentThread);
    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

int do_ForkExec(char * filenameAddress){
    printf("currentThread before Fork = %p\n", currentThread);
	Thread *thread = new Thread("thread processus");
    thread->Fork(execProcess, (int)filenameAddress);
	//printf("File : %x\n", filenameAddress);
    scheduler->Print();
    currentThread->Yield();
    printf("Forked thread = %p\n", thread);
	return 0;
}