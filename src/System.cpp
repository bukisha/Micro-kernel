#include "System.h"
#include "stdio.h"
#include "PCB.h"
#include "Schedule.h"
#include "lock.h"
#include "Thread.h"
#include "dos.h"
#include<iostream.h>
#include "conio.h"
//static volatile PCB* novi;

volatile int System::expDisp=0;
IR System::oldRutine=0;
volatile PCB* System::running = 0;
Thread* System::starting=0;
Lista* System::sleepList=0;
IThread* System::Idle=0;
volatile int System::startFlag=1;



//unsigned teBP,teSS,teSP;
void interrupt System::myTimer() {
static volatile unsigned int teSS,teSP,teBP;
	 static volatile PCB *novi;
	 
			
	 if (!expDisp) {
	(*oldRutine)();
	
	//	printf("\nPozvao	sam implicitDispatch");
	
		//sleepList->writeList();	
//	lock;
		  
		tick();
	//==========================================================
	//ovaj deo sredjuje listu uspavanih niti 

		if(sleepList->first) {
		Element* temp=sleepList->first;
		while(temp) {   //za svaki el u sleepListi se odradjuju ove radnje
				Element* pom=temp->next;
					temp->pcb->sleepCNT+=55;
					if(temp->pcb->sleepCNT>temp->pcb->myTimeToSleep) {   //ako je odpsavala dovoljno ovo se obavi
						temp->pcb->sleepCNT=0;//resetuje se counter
						temp->pcb->compleated=1;
						temp->pcb->state=PCB::READY;
						Scheduler::put(temp->pcb);
					//	printf("\nVratio u scheduler ");
					//	printf(temp->pcb->myThread->getName());
						sleepList->remove(temp->pcb->myID);
							temp=pom;				
					}	else {
						temp=temp->next;
					}
		}
	}	
	
	//odradi
	//==============================================================

		
	
	if (running->myTimeSlice != 0) running->myPassedTime += 55;
	
	if ((running->myPassedTime < running->myTimeSlice) || (running->myTimeSlice == 0) ) 
		return;
			
	if( (running->state == PCB::READY) && (running != Idle->myPCB) ) 
		Scheduler::put((PCB*)running);

	while(1)
	{
		novi = Scheduler::get();
		
		if (novi == 0) {
							novi = Idle->myPCB;
			}
		
	
		if (novi->state != PCB::READY) continue;
		asm {
			mov teSP, sp
			mov teSS, ss
			mov teBP, bp
		}
		running->mySP = teSP;
		running->mySS = teSS;
		running->myBP = teBP;
		
		
		
		running = novi;
		teBP = running->myBP;
		teSP = running->mySP;
		teSS = running->mySS;
		asm {
			mov sp, teSP
			mov ss, teSS
			mov bp, teBP
		}
			running->myPassedTime = 0;
		break;
		}
	return;
	 
	 
	 }  else {
 expDisp = 0;
	
 if( (running->state == PCB::READY) && (running != Idle->myPCB) ) 
		Scheduler::put((PCB*)running);
	
	while(1)
	{
		novi = Scheduler::get();
		
		if (novi == 0) {
		
				novi = Idle->myPCB;
			
			}
		
		if (novi->state != PCB::READY) continue;
		
		
		asm {
			mov teSP, sp
			mov teSS, ss
			mov teBP, bp
		}
		running->mySP = teSP;
		running->mySS = teSS;
		running->myBP = teBP;
		running = novi;
		teBP = running->myBP;
		teSP = running->mySP;
		teSS = running->mySS;
		asm {
			mov sp, teSP
			mov ss, teSS
			mov bp, teBP
		}
			running->myPassedTime = 0;
	 break;	
	}

return;
}
	
	
} //end mytimer

void System::initiallize() {
//zavrsi
lock;

oldRutine=setINT();
starting=new Thread("starting",0,1);
starting->myPCB->state=PCB::READY;
sleepList=new Lista();
running=starting->myPCB;
Idle=new IThread(); 
Idle->start();

unlock;


}

void System::ShutDown() {
lock;
if( (PCB*)running!=starting->myPCB){unlock;return;}
	//while(PCB::ThreadPool->size()>2);

	
//printf("\nGasim se");	
		restoreINT();
		delete starting;	
	delete Idle;
unlock;

	
//zavrsi
}

void System::wrapper() {
	//printf("\nPozvao wrapper");
	running->myThread->run();
	lock;
	running->state=PCB::OVER;
	PCB::ThreadPool->remove(running->myID);
	PCB* temp;
	while(running->waitLista->size()  >  0) {
	temp=running->waitLista->get();
	temp->state=PCB::READY;
	Scheduler::put(temp);
	}
	if(running->myParent) {
		running->myParent->myPCB->numOfChildren-=1;
		if(running->myParent->myPCB->numOfChildren==0 && running->myParent->myPCB->state==PCB::WAITCHILDREN) {
			running->myParent->myPCB->state=PCB::READY;
			Scheduler::put(running->myParent->myPCB);
		}
	}
	//printf("\nzavrsila se %s",running->myThread->getName());
	System::dispatch();
unlock;
	//zavrsi
}

void System::dispatch()  {
lock;	
	//printf("\npozvao dispatch\n");
expDisp=1;
myTimer();
expDisp=0;
unlock;
//zavrsi
}

IR System::setINT()  {
	lock;
	
	unsigned  int oldSEG,oldOFF;
	unsigned int tempSEG,tempOFF;
	IR old;
	
	tempSEG=FP_SEG(&(System::myTimer));
	tempOFF=FP_OFF(&(System::myTimer));
	
	
	asm {
		
		push ax
		push es
		mov ax,0
		mov es,ax

		mov ax,word ptr es:0022h
		mov word ptr oldSEG,ax
		mov ax,word ptr es:0020h
		mov word ptr oldOFF,ax

		

		mov ax,tempSEG
		mov word ptr es:0022h, ax
		mov ax,tempOFF
		mov word ptr es:0020h, ax

   
		
		pop es
		pop ax


	}


old=(IR)MK_FP(oldSEG,oldOFF);
unlock;

	return old;
}

void System::restoreINT() {
lock;
	unsigned int tempSEG,tempOFF;
tempSEG=FP_SEG(oldRutine);
tempOFF=FP_OFF(oldRutine);
	
	
	asm {
		
			push ax
			push es
			mov ax,0
			mov es,ax
			
			mov ax,word ptr tempSEG
			mov es:0022h,ax
			mov ax,word ptr tempOFF
			mov es:0020h,ax
						

			pop es
			pop ax


	}//end asm
unlock;


}
/*
void System::test() {
Lista* proba=new Lista();
	Thread* t1=new Thread("prva",4096,1);
	proba->put(t1->myPCB);
	Thread* t2=new Thread("druga",4096,1);
	proba->put(t2->myPCB);
	Thread* t3=new Thread("treca",4096,1);
	proba->put(t3->myPCB);
	proba->remove(1);
	proba->remove(3);
	proba->put(t1->myPCB);
	cout<<"\n"<<proba->get()->myThread->getName()<<endl;
	cout<<"\n"<<proba->get()->myThread->getName()<<endl;
	cout<<"\n"<<proba->get()->myThread->getName()<<endl;
	//for(long y=0;y<10000;y++);


}*/