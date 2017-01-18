#ifdef CHANGED
#ifndef USERSEM_H
#define USERSEM_H


class UserSem {
	public:
	
		UserSem();
		
		~UserSem();
	
		int do_SemCreate(int n);

		void do_SemP(int s);

		void do_SemV(int s);

		void do_SemDestroy(int s);
};
#endif //USERSEM
#endif //CHANGED