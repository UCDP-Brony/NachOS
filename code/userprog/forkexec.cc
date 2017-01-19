#include "forkexec.h"
#include "syscall.h"
#include "system.h"
#include "addrspace.h"
#include "threadargs.h"

void execProcess(int filenameAddress){
	printf("File : %x\n", filenameAddress);
	char * fileName = (char *)filenameAddress;
    OpenFile *executable = fileSystem->Open (fileName);
    AddrSpace *space;
    if (executable == NULL)
    {
	  synchConsole->SynchPutString("executable = NULL\n");
	  return;
    }

    space = new AddrSpace (executable);
    currentThread->space = space;
    
    delete executable;		// close file

    space->InitRegisters ();	// set the initial register values
    space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

int do_ForkExec(char * filenameAddress){
	Thread *thread = new Thread("thread utilisateur");

	thread->Fork(execProcess, (int)filenameAddress);
	currentThread->Yield();
	return 0;
}