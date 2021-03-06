#include "forkexec.h"
#include "syscall.h"
#include "system.h"
#include "addrspace.h"
#include "threadargs.h"


//----------------------------------------------------------------------
// execProcess (int filenameAddress)
//      initialises the address space of the new thread by loading the 
//      program found at filenameAddress then runs the program. 
//      Warns the user if the program is invalid.
//----------------------------------------------------------------------
void execProcess(int filenameAddress){
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

//----------------------------------------------------------------------
// do_ForkExec (char * filenameAddress)
//      Creates a new Thread that will run the program found at filenameAddress.
//----------------------------------------------------------------------
int do_ForkExec(char * filenameAddress){
	Thread *thread = new Thread("thread processus");
    thread->Fork(execProcess, (int)filenameAddress);
    currentThread->Yield();
	return 0;
}