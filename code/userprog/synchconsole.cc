#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static Semaphore *writing;
static Semaphore *reading;

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	writing = new Semaphore("currently writing", 1);
	reading = new Semaphore("currently reading", 1);
	console = new Console (readFile, writeFile, ReadAvail, WriteDone, 0);
}

SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
	delete writing;
	delete reading;
}

void SynchConsole::SynchPutChar(const char ch)
{
	writing->P();
	console->PutChar (ch);
	writeDone->P ();
	writing->V();
}

int SynchConsole::SynchGetChar()
{
	reading->P();
	readAvail->P ();	// wait for character to arrive
	int tmp;
	if (console->Feof())
		tmp = EOF;
	else
		tmp = console->GetChar ();
	reading->V();
	return tmp;
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
	char c;
	for (i = 0; i < n && !end; i++){
		c = SynchGetChar();
		if (c==EOF)
			end = 1;
		else
			s[i] = c;
		if (c=='\0' || c =='\n')
			end =1;
	}
	s[i] ='\0';
}

void SynchConsole::SynchPutInt(int n){
	char str[MAX_STRING_SIZE];
	snprintf(str, MAX_STRING_SIZE, "%d", n);
	SynchPutString(str);
}

void SynchConsole::SynchGetInt(int *n){
	char str[MAX_STRING_SIZE];
	SynchGetString(str,MAX_STRING_SIZE);
	*n = atoi(str);
}

#endif // CHANGED