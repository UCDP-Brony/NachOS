#ifndef USERTHREAD_H
#define USERTHREAD_H
extern int do_UserThreadCreate(int, int); 				// create a new Thread and returns its address
extern void do_UserThreadExit(); 						// do a broadcast on the lock of the Thread then finish the Thread.
extern void do_UserThreadJoin(unsigned int address);	// the thread calling this waits for the thread contained in address to finish
#endif //USERTHREAD_H