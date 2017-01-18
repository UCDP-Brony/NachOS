// synch.cc 
//      Routines for synchronizing threads.  Three kinds of
//      synchronization routines are defined here: semaphores, locks 
//      and condition variables (the implementation of the last two
//      are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      Initialize a semaphore, so that it can be used for synchronization.
//
//      "debugName" is an arbitrary name, useful for debugging.
//      "initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore (const char *debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
//      De-allocate semaphore, when no longer needed.  Assume no one
//      is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore ()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
//      Wait until semaphore value > 0, then decrement.  Checking the
//      value and decrementing must be done atomically, so we
//      need to disable interrupts before checking the value.
//
//      Note that Thread::Sleep assumes that interrupts are disabled
//      when it is called.
//----------------------------------------------------------------------

void
Semaphore::P ()
{
    IntStatus oldLevel = interrupt->SetLevel (IntOff);	// disable interrupts

    while (value == 0)
      {				// semaphore not available
	  queue->Append ((void *) currentThread);	// so go to sleep
	  currentThread->Sleep ();
      }
    value--;			// semaphore available, 
    // consume its value

    (void) interrupt->SetLevel (oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
//      Increment semaphore value, waking up a waiter if necessary.
//      As with P(), this operation must be atomic, so we need to disable
//      interrupts.  Scheduler::ReadyToRun() assumes that threads
//      are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V ()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel (IntOff);

    thread = (Thread *) queue->Remove ();
    if (thread != NULL)		// make thread ready, consuming the V immediately
	scheduler->ReadyToRun (thread);
    value++;
    (void) interrupt->SetLevel (oldLevel);
}

//----------------------------------------------------------------------
// Lock::Lock
// 		Initialize a mutex, so that it can be used for synchronization
//      "debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------

Lock::Lock (const char *debugName)
{
	holder = NULL;
	S = new Semaphore("mutex-like", 1);
}

//----------------------------------------------------------------------
// Lock::Lock
//      De-allocate mutex, when no longer needed.  Assume no one
//      is still waiting on the mutex!
//----------------------------------------------------------------------

Lock::~Lock ()
{
	delete S;
}

//----------------------------------------------------------------------
// Lock::Acquire
//		We have to save currentThread to make sure that no thread but us can release the lock
//----------------------------------------------------------------------

void
Lock::Acquire ()
{
	S->P();
	//have to save for checking later on release
	holder = currentThread;
}

//----------------------------------------------------------------------
// Lock::Release
// 		Must be called after an Acquire has been done and by the thread who previously called it, otherwise nothing will happen
//----------------------------------------------------------------------

void
Lock::Release ()
{
	//can't release for another thread, we have to check
	if(holder == currentThread){
		holder = NULL;
		S->V();
	}
	//else do nothing
	
}

//----------------------------------------------------------------------
// Condition::Condition
//		Initialize a condition, so that it can be used for synchronization
//		"debugName" is an arbitrary name, useful for debugging.
//----------------------------------------------------------------------

Condition::Condition (const char *debugName)
{
	waitList = new List;
}

//----------------------------------------------------------------------
// Condition::Condition
//      De-allocate condition, when no longer needed.  Assume no one
//      is still waiting on the condition!
//----------------------------------------------------------------------

Condition::~Condition ()
{
	delete waitList;
}

//----------------------------------------------------------------------
// Condition::Wait
//		"conditionLock" must be an initialized mutex, currently locked
//----------------------------------------------------------------------

void
Condition::Wait (Lock * conditionLock)
{
	Semaphore *S = new Semaphore("used to lock the thread",0);
	waitList->Append(S);
	conditionLock->Release();
	S->P();
	conditionLock->Acquire();
}

//----------------------------------------------------------------------
// Condition::Lock
//		"conditionLock" currently not used, might require a fix later 
//----------------------------------------------------------------------

void
Condition::Signal (Lock * conditionLock)
{
	//nothing to do if the list is empty
	if(!waitList->IsEmpty()){
		//fetch the semaphore of a waiting thread, then increase it to free the locked thread
		Semaphore *S = (Semaphore *)waitList->Remove();
		S->V();
	}
	//else do nothing
}

//----------------------------------------------------------------------
// Condition::Broadcast
//		"conditionLock" must be an initialized mutex, currently locked
//----------------------------------------------------------------------

void
Condition::Broadcast (Lock * conditionLock)
{
	while(!waitList->IsEmpty()){
		Signal(conditionLock);
	}
}
