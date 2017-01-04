How to do a system call :

userprog/syscall.h : ajouter #define SC_[APPEL], ainsi que la fonction correspondante.
test/start.S : ajouter la définition de la fonction.
	notes : le numéro d'appel système est placé dans r2, le premier argument dans r4.
	L'argument est implicitement converti en int, il faut donc penser à le reconvertir à sa sortie du registre.
userprog/exception.cc : mise en place du handler. On ajouter un case dans le switch ExceptionHandler(ExceptionType which).
	case SC_[EVENT] : {
		doStuff; //généralement interrupt->event();
	}
