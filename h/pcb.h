#ifndef _pcb_h_
#define _pcb_h_
#include "Thread.h"
#include "System.h"
#include "Lista.h"
#include "Semaphor.h"
#include "event.h"
#include "KernEv.h"
class PCB {
friend class System;
friend class Lista;
friend class IThread;
friend class Thread;

public:
	PCB(char* name,StackSize s,Time t,Thread* tt);//konstruktor
	~PCB();//destruktor
	void buildStack();  //pravi stack
	int waitReturn;// promenljiva kroz koju se vraca vrednost semafora
	static const int NEW,READY,BLOCKED,OVER,WAITCHILDREN ; //stanja niti u kojima se ona moze nalaziti
 	volatile int state;    //stanje niti
	static ID globalID;   //globadlna promenljiva za identifikaciju
	static Lista* ThreadPool;  //lista svih pcbova
	//===================================================================
	Thread* myThread;       //pokazivac na objekat niti ciji je ovo PCB
	ID myID;   //identifikator za konkretnu nit
	TName myName;   //ime niti
	unsigned int myTimeSlice; // vremenski kvant
	
volatile int sleeping;//indikator da li je nit uspavana


unsigned int myTimeToSleep;  //vrednost koliko treba da se spava
volatile unsigned int sleepCNT;   //brojac koliko je svaka nit spavala	
volatile unsigned int myPassedTime;  //merac vremena koliko se nit izvrsavala
	unsigned myBP,mySS,mySP;  //promenljive u kojima cu da cuvam ss i sp za datu nit
	StackSize myStackSize;    //velicina steka za doticnu nit
	unsigned* myStack;   //pokazivac na vrh kontrolnog steka
	Lista* waitLista;
	int compleated;   //indikator za waittocompletefunkciju
	int awaken;    //indikator da li je nit budjena ranije ili ne 
	
	Semaphore* mySem;  // semafor na kome je blokirana ova nit
	Event* myEvent;   //dogadjaj na koji ceka ova nit
	Thread* blockedOnThread;//nit na kojoj je blokira ova nit
	
	
	int numOfChildren;    //broj dece koju je stvorila ova nit
	Thread* myParent;   //nit roditelj za ovu nit
	//==================================================================
	
	
};   //end PCB

#endif 