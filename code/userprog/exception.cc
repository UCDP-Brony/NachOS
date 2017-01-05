// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	#ifndef CHANGED // Noter le if*n*def
	if ((which == SyscallException) && (type == SC_Halt)) {
		DEBUG('a', "Shutdown, initiated by user program.\n");
		interrupt->Halt();
	} else {
		printf("Unexpected user mode exception %d %d\n", which, type);
		ASSERT(FALSE);
	}
	#else // CHANGED
	if (which == SyscallException) {
		switch (type) {
			case SC_Halt: {
				DEBUG('a', "Shutdown, initiated by user program.\n");
				interrupt->Halt();
				break;
			}
			case SC_PutChar: {
				DEBUG('a', "Call to PutChar \n");
				interrupt->PutChar((char)machine->ReadRegister(4));
				break;
			}
			case SC_copyStringFromMachine: {
				DEBUG('a', "Call to copyStringFromMachine \n");
				interrupt->copyStringFromMachine(machine->ReadRegister(4), (char *)machine->ReadRegister(5), (unsigned)machine->ReadRegister(6));
				//copyStringFromMachine( int from, char *to, unsigned size);
				break;
			}
			case SC_SynchPutString: {
				DEBUG('a', "Call to SynchPutString \n");
				char to[MAX_STRING_SIZE]; 
				interrupt->copyStringFromMachine(machine->ReadRegister(4), to, MAX_STRING_SIZE);
				interrupt->SynchPutString(to);
				break;
			}
			case SC_Exit: {
				DEBUG('a', "Call to Exit\n");
				int valReturn = machine->ReadRegister(4);
				DEBUG('a',"Program finished with return value of %d \n",valReturn);
				printf("exiting \n"); //Necessaire ?
				interrupt->Halt();
				break;
			}
			case SC_SynchGetChar: {
				DEBUG('a', "Call to SynchGetChar \n");
				machine->WriteRegister(2,synchConsole->SynchGetChar());
				break;
			}
			case SC_SynchGetString:{
				DEBUG('a', "Call to SynchGetChar \n");
				char str[MAX_STRING_SIZE];
				int l = machine->ReadRegister(5);
				synchConsole->SynchGetString(str,l);
				interrupt->copyStringToMachine(machine->ReadRegister(4),str,l);
				break;
				
			}
			case SC_SynchPutInt:{
				synchConsole->SynchPutInt(machine->ReadRegister(4));
				break;
			}
			case SC_SynchGetInt:{
				int res;
				synchConsole->SynchGetInt(&res);
				//int = 2 octets sur anciennes machines, 4 sur 32 bits
				//Probleme possible sur 64 bits, 8 octets, max 4
				machine->WriteMem(machine->ReadRegister(4),sizeof(int),res);
				break;
				
			}
			default: {
				printf("Unexpected user mode exception %d %d\n", which, type);
				ASSERT(FALSE);
			}
		}
	}
	#endif // CHANGED
	UpdatePC();
}
