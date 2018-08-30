#include "kernEv.h"
#include "event.h"
#include "System.h"
#include "Lista.h"
#include "PCB.h"
#include "Schedule.h"
#include "IVTE.h"
#include "lock.h"



KernelEv::KernelEv(IVTNo n) {
//lock;
	value=0;
myPCB=(PCB*)System::running;
nextEv=0;

entry=n;
insertEv(this,this->entry);
//unlock;
}


KernelEv::~KernelEv() {
	
	
	if(myPCB->state==PCB::BLOCKED) {
		myPCB->state=PCB::READY;
		myPCB->myEvent=0;
		myPCB->awaken=1;
		Scheduler::put(myPCB);
	
	}
	

}

void KernelEv::insertEv(KernelEv* kev,int e) {   //ovde si zajebao ubacivanje
lock;
IVTEntry* tempIVT=IVTEntry::getIVT(e);

if(tempIVT->firstEv) {
	kev->nextEv=tempIVT->firstEv;
	tempIVT->firstEv=kev;
} else {
	tempIVT->firstEv=kev;
}

unlock;
}
//=============================================================
void removeEv(KernelEv* kev) {
//lock;
	IVTEntry* tempIVT=IVTEntry::getIVT(kev->entry);
	KernelEv* tempEv=tempIVT->firstEv;	
	
	if(tempEv==kev) {
		if(!tempEv->nextEv) {
			tempIVT->firstEv=0;
		} else {
			tempIVT->firstEv=tempIVT->firstEv->nextEv;
		}
	}
	
	while(tempEv->nextEv!=kev) tempEv=tempEv->nextEv;
		tempEv->nextEv=kev->nextEv;
		kev->nextEv=0;
//unlock;
}

int KernelEv::wait() {
	//lock;
	if(myPCB->myThread->getId()!=System::running->myID) return -1;
			
	if(value==0) {	
		value=-1;
		System::running->state=PCB::BLOCKED;
		System::running->myEvent->myImpl=this;
		//System::dispatch();
	} else {
		if(value==-1) {
		value=0;	
		//System::running->awaken=1;
		}
	}
	System::dispatch();
	//unlock;
	return System::running->awaken;
}

void KernelEv::signal() {
	//lock;
	if(value==-1) {
		value=0;
		myPCB->state=PCB::READY;
		myPCB->myEvent=0;
		myPCB->awaken=1;
		Scheduler::put(myPCB);
		
	} else {
	value=1;
	}
	if(nextEv) nextEv->signal();
	//unlock;
}