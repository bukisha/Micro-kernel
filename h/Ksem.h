#ifndef _Ksem_h_
#define _Ksem_h_
#include "Lista.h"
#include "Semaphor.h"
class KernelSem {

public:
	
	KernelSem(int x,Semaphore* s);
	~KernelSem();
    virtual	int wait();
	virtual void signal();

	
	Semaphore* sem;   //semafor cija je ovo implementacija
	int value;
	Lista* blockedOnMe;

};



#endif