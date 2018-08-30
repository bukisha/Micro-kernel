#include "Thread.h"
#include "pcb.h"
#include "Lista.h"
#include "stdio.h"   //za potrebe testiranja;
#include <iostream.h>
#include "System.h"
#include "IThread.h"
#include "Schedule.h"
#include "lock.h"
#include "dos.h"

extern int userMain(int argc,char* argv[]);

class UserMainT : public Thread {
public:
int ret;
int myARGC;
char** myARGV;
UserMainT(int argc,char* argv[]) : Thread("userMainThread",defaultStackSize,0) {
	
	ret=0;
	myARGC=argc;
	myARGV=argv;
}


	void run() {
	ret=userMain(myARGC,myARGV);
	
	}
};

int main(int argc,char* argv[])  {
int i;
lock;
UserMainT* MT=new UserMainT(argc,argv);
unlock;
	MT->start();
	
	System::initiallize();
	while(MT->myPCB->state!=PCB::OVER);
//while(PCB::ThreadPool->size()>2);
	//PCB::ThreadPool->writeList();
	//printf(Scheduler::get()->myThread->getName());
//i=userMain();
 // for(long x=0;x<20000;x++)
	//  for(long y=0;y<300000;y++);
	//printf(System::sleepList->get()->myThread->getName());
	System::ShutDown();
i=MT->ret;
	
	

	delete MT;
	

	return i;
	
}   // end main