#ifndef _IVTE_h_
#define _IVTE_h_

#include "event.h"
#include "Lista.h"

#define PREPAREENTRY(ent,intFlag)\
void interrupt interruptRoutine##ent();\
IVTEntry entry##ent(ent, interruptRoutine##ent);\
void interrupt interruptRoutine##ent(){\
	entry##ent.signal();\
	if(intFlag)entry##ent.oldRoutine();\
}
typedef void (interrupt *IR)(); 
class KernelEv;

class IVTEntry {
friend class Lista;
public:
	int entryNo;
	IR oldR,newR;
	KernelEv* firstEv;
	//=============================================
	static IVTEntry* firstIVT;
	static IVTEntry* lastIVT;
	IVTEntry *nextIVT,*prevIVT;
	static void add(IVTEntry* ivt);//ublanc novi ivt tamo gde treba
	static void remove(IVTEntry* ivt); //izbaci ivt koji dobije kao argument iz lsite ako ga ima u lsiti ako ne onda nsita
	static IVTEntry* getIVT(int x); //vraca pokazivac na ivtentry na ulazu x     
	
	//==============================================
	
IVTEntry(int,IR);
~IVTEntry();

void signal();

void oldRoutine() {(*oldR)();}  //preko pokazivaca se poziva stara prekidna rutina 

void setINT();
void restoreINT();
};

#endif