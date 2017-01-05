#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}

void SynchConsole::SynchPutChar(const char ch)
{
	console->PutChar (ch);
	writeDone->P ();
}

char SynchConsole::SynchGetChar()
{
	readAvail->P ();	// wait for character to arrive
	return console->GetChar ();
}

void SynchConsole::SynchPutString(const char s[])
{
	int i = 0;
	while (s[i] != '\0'){
		SynchPutChar(s[i]);
		i++;
	}
}

void SynchConsole::SynchGetString(char *s, int n)
{
	int i;
	int end = 0;
	for (i = 0; i < n && !end; i++){
		s[i] = SynchGetChar();
		if(s[i] == '\n')
		{
			end = 1;
		}
	}
	s[i+1] = '\0';
}

void SynchConsole::SynchPutInt(int n){
	char str[MAX_STRING_SIZE];
	snprintf(str, MAX_STRING_SIZE, "%d", n);
	SynchPutString(str);
}

void SynchConsole::SynchGetInt(int *n){
	char str[MAX_STRING_SIZE];
	SynchGetString(str,MAX_STRING_SIZE);
	*n = (int) str;
}

#endif // CHANGED