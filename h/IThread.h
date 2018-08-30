#ifndef _ithread_h_
#define _ithread_h_

#include "Thread.h"
class IThread : public Thread {

public :
	IThread();
	void start();
virtual	void run() ;

};

#endif 