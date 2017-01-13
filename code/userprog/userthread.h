#ifndef USERTHREAD_H
#define USERTHREAD_H
extern int do_UserThreadCreate(int, int);
extern void do_UserThreadExit();
extern void do_UserThreadJoin(unsigned int address);
#endif //USERTHREAD_H