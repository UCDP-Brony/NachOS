#ifdef CHANGED
#include "copyright.h"
#include "usersem.h"
#include "syscall.h"
#include "system.h"
#include "synch.h"


	Semaphore* semArray[MAX_SEMAPHORES]; //you can change MAX_SEMAPHORES in system.h
	Lock* myLock;
	
	UserSem::UserSem(){
		
		myLock = new Lock("Lock used for user semaphore");
		int i;
		for(i = 0; i<MAX_SEMAPHORES;i++){
			semArray[i] = NULL;
		}
	}
	
	UserSem::~UserSem(){
		int i;
		for(i = 0; i<MAX_SEMAPHORES;i++){
			if (semArray[i] != NULL)
				delete semArray[i];
		}
		delete myLock;
	}
	
	
	//sem_t is int
	int UserSem::do_SemCreate(int n){
		myLock->Acquire();
		int i = 0;
		while(i<MAX_SEMAPHORES){
			if (semArray[i] == NULL){
				Semaphore *temp = new Semaphore("usersem",n);
				semArray[i] = temp;
				myLock->Release();
				return i;
			}
		}
		myLock->Release();
		return -1; //no space avalaible
	}

	void UserSem::do_SemP(int s){
		if(-1<s && s<MAX_SEMAPHORES){//make sure that the parameter is legit
			Semaphore *temp = semArray[s];
			temp->P();
		}
		
	}

	void UserSem::do_SemV(int s){
		if(-1<s && s<MAX_SEMAPHORES){//make sure that the parameter is legit
			Semaphore *temp = semArray[s];
			temp->V();
		}
	}

	void UserSem::do_SemDestroy(int s){
		if(-1<s && s<MAX_SEMAPHORES){
			delete semArray[s];
		}
	}
	
	
#endif // CHANGED



