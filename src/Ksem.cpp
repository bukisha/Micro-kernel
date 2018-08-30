#include "Ksem.h"
#include "Semaphor.h"
#include "Lista.h"
#include "Lock.h"
#include "System.h"
#include "PCB.h"
#include "Schedule.h"

extern int semPreempt;
KernelSem::KernelSem(int x,Semaphore* s) {
	sem=s;
	value=x;
	blockedOnMe=new Lista();

}

KernelSem::~KernelSem() {

delete blockedOnMe;

}

int KernelSem::wait() {

	value--;
if(value<0) {
	System::running->state=PCB::BLOCKED;
	System::running->mySem=this->sem;
	blockedOnMe->put((PCB*)System::running);
	System::dispatch();
	
}else {
	System::running->awaken=1;//ubacio ja
if(semPreempt) System::dispatch();
}

return System::running->awaken;

//zavrsi
}



void KernelSem::signal() {
value++;
if(value<=0) {
	PCB* p=blockedOnMe->get();
	if(p) {
		p->state=PCB::READY;
		p->awaken=1;
		p->mySem=0;
		Scheduler::put(p);
		}
} else{
	if(semPreempt) System::dispatch();}
//zavrsi dodaj semPreempt i ostala govna i ovde i u wait
}