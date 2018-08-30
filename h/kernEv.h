#ifndef _kernEv_h_
#define _kernEv_h_
#include "PCB.h"
#include "Lista.h"
#include "Event.h"
#include "kernEv.h"

class KernelEv {
friend class Thread;
public:
	int value;//vrednost semaforaEVENT
	PCB* myPCB;//pokazivac na pcb niti koja je blokirana na ovom eventu
	IVTNo entry;
	
	KernelEv* nextEv;
	
	static void insertEv(KernelEv* kev,int e);  //ubaci kernEv u listu cekanaj na odgovarajuci ivt ulaz
	void removeEv(KernelEv* kev);
	
	
	KernelEv(IVTNo x);
	~KernelEv();


	int wait();
	void signal();
//zavrsi

};
#endif 