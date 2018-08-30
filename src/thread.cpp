#include "Thread.h"
#include "PCB.h"
#include <iostream.h>
#include <stdio.h>
#include "Schedule.h"
#include "lock.h"
#include "Lista.h"
#include "string.h"//zbog poredjenjastringova
#include "conio.h"
#include "Ksem.h"
#include "Semaphore.h"
#include "event.h"
#include "kernEv.h"
Thread::Thread(TName n,StackSize s,Time t) {
	lock;
	//cprintf("\nuso u thread konstruktor\n");
	myPCB=new PCB(n,s,t,this);  //this je pokazivac na thread ciji PCB pravimo
	unlock;
}
Thread::~Thread()  {
	lock;
//printf("\nUnistavam %s",getName());
	waitToComplete();
	//PCB::ThreadPool->remove(myPCB->myID);
	delete myPCB;
	
	unlock;

}      //dovrsi destruktor

ID Thread::start()  {
lock;
//if(myPCB->state==PCB::OVER) {
//unlock;
//return 0;
//}	
//printf("\nstartujem");
	ID tempID;
	myPCB->state=PCB::READY;
	
		myPCB->buildStack();
	Scheduler::put(myPCB);
	tempID=this->getId();
	
	
	unlock;
	return tempID;

}   //dovrsi

//=======================================================================================
//staticki waitToCompleate() 
int Thread::waitToComplete(ID id) {
	lock;
	Element* temp;
	int foundIt=0;
	temp=PCB::ThreadPool->first;
	if(!temp) {unlock;return -1;}
	while(temp) { 
		if(temp->pcb->myID==id) {
			foundIt=1;
			break;
			//temp=temp->next;
		}else{
			temp=temp->next;}
	}
	if(foundIt==0) {unlock;return -1;}
	int ret;
	ret=temp->pcb->myThread->waitToComplete();
	unlock;
	return ret;
}  //dovrsi   
int Thread::sleep(Time timeToSleep)  {
	lock;
	System::running->state=PCB::BLOCKED;
	//printf("\nUspavao sam: %s",System::running->myThread->getName());
	System::running->myTimeToSleep=timeToSleep*55;
	System::running->sleeping=1;
	System::sleepList->put((PCB*)System::running);
	System::dispatch();
	unlock;
	return System::running->awaken;

}   //dovrsi

int Thread::wakeUp(ID id) {
	lock;
	Element* temp=PCB::ThreadPool->first;
	if(!temp) {unlock;return -1;}
	int foundIt=0;
	while(temp) {
		if(temp->pcb->myID==id) {  
			foundIt=1;	
			break;
		}else {
			temp=temp->next;}
} //kad dodjem do ovde nasao sam nit sazadatimidjem u threadPool-u
	if(foundIt==0) {unlock;return -1;}
	if(temp->pcb->state!=PCB::BLOCKED ) {unlock;return 0;}
if(temp->pcb->sleeping==1) {        //ako sam uspavan
	System::sleepList->remove(id);
	temp->pcb->myTimeToSleep=0;
	temp->pcb->sleepCNT=0;
	temp->pcb->sleeping=0;
	temp->pcb->awaken=1;
	

}


if(temp->pcb->mySem) {           //ako sam blokiran na semaforu
	temp->pcb->mySem->myImpl->value++;
	temp->pcb->mySem->myImpl->blockedOnMe->remove(id);
	temp->pcb->awaken=0;	
	
	
}
if(temp->pcb->myEvent) {                  //ako cekam da se desi neki dogadjaj
	Event* old=temp->pcb->myEvent;
	temp->pcb->myEvent->myImpl->value=0;    //ovo nije gotovo obrati paznju
	temp->pcb->myEvent=0;
	temp->pcb->awaken=0;
	
}

if(temp->pcb->blockedOnThread) {
		temp->pcb->blockedOnThread->myPCB->waitLista->remove(temp->pcb->myID);
		temp->pcb->blockedOnThread=0;
		temp->pcb->awaken=0;
}

//zavrsi dodaj za dogadjaje sta se radikad se budi nit
temp->pcb->state=PCB::READY;
Scheduler::put(temp->pcb);




unlock;
return 1;//temp->pcb->awaken;  //povratna vrednost sredi
}    //dovrsi

//============================================

int Thread::waitForChildren()  {
	lock;
	if(System::running->numOfChildren==0) {unlock;return -1;}
	//lock;

	System::running->state=PCB::WAITCHILDREN;
//	printf("broj dece: %d",System::running->numOfChildren);
//	for (long x=0;x<200000;x++);
	System::dispatch();
	unlock;
	return System::running->awaken;   //pozabavi se sa ovim povratnim vrednostima kad proradi sama metoda ;D


}    //dovrsi

//================================================


//obican waitToCompleate()
int Thread::waitToComplete() { 
	lock;
	if((myPCB->state==PCB::OVER) ) {  //mozda treba jos uslova da se doda
	unlock;							//kao kod ilije
	return 1;
	} 
	if(myPCB==(PCB*)System::running) {
	unlock;
	return 1;
	}
	if(this==System::starting) {
	unlock;
	return 1;
	}
	if(this==System::Idle)  {
	unlock;
	return 1;
	}
	
	System::running->state=PCB::BLOCKED;
	System::running->blockedOnThread=myPCB->myThread;
	myPCB->waitLista->put((PCB*)System::running);
	
	System::dispatch();
	unlock;
	return myPCB->awaken;
}  //dovrsi
//=======================================================================================


ID Thread::getId() {return myPCB->myID;}    
TName Thread::getName()  {return myPCB->myName;} 

//============================================================
Thread* Thread::getThreadById(ID id) {
	lock;
	for(Element* temp=PCB::ThreadPool->first;temp;temp=temp->next) {
		if(temp->pcb->myID==id) {
		unlock;
		return temp->pcb->myThread;
		}
		
	}
	unlock;
	return 0;
} //=========================================================

ID Thread::getIdOf(TName name)  {
lock;
	for(Element* temp=PCB::ThreadPool->first;temp;temp=temp->next) {
		if(!strcmp(temp->pcb->myName,name)) {
		unlock;
		return temp->pcb->myID;
		}
		
	}
	unlock;
	return 0;
} 


//============================================================
TName Thread::getName(ID id) {
lock;
	for(Element* temp=PCB::ThreadPool->first;temp;temp=temp->next) {
		if(temp->pcb->myID==id) {
		unlock;
		return temp->pcb->myName;
		}
	}
	unlock;
	return 0;
} //===============================================================

void dispatch() {
	lock;
	System::dispatch();
	unlock;
}

