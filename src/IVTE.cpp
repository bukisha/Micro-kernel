#include "IVTE.h"
#include "lock.h"
#include "dos.h"
#include "kernEv.h"
#include "string.h"
#include "stdio.h"
#include "System.h"
#include "Lista.h"
#include "PCB.h"

IVTEntry* IVTEntry::firstIVT=0;
IVTEntry* IVTEntry::lastIVT=0;

IVTEntry::IVTEntry(int e,IR n) {
lock;
nextIVT=0;//nema sledbenika kad se napravi
prevIVT=0;//nema prethodnika kad se napravi
entryNo=e;
newR=n;
//postavi prekidnu rutinu na mesto
setINT();
add(this);
firstEv=0;//nema dogadjaja blokiranih an ovom ulazu kad se napravi


unlock;
}

IVTEntry::~IVTEntry() {
lock;
//izlancaj ga iz liste ,mozda i en mroas ali proveri

remove(this); //uklanjam IVT iz globalne liste IVTova

KernelEv* oldEv;

while(firstEv) {
	oldEv=firstEv;
	firstEv=firstEv->nextEv;
	delete oldEv;

}

restoreINT();
//remove(this);
unlock;

}



void IVTEntry::signal() {   //na jednom dogadjaju je blokirana samo jedna nit

		if(firstEv) firstEv->signal();

System::dispatch();

//budi sve koji su se registrovali da cekaju ovaj dogadjaj
//i izbacuje ih iz lanca svih KernelEvova u sistemu

}

void IVTEntry::setINT() {
lock;
unsigned newSEG,newOFF;	
unsigned oldSEG,oldOFF;

newSEG=FP_SEG(*newR);
newOFF=FP_OFF(*newR);
	int entry=4*entryNo;
asm {
	push ax
	push es
	push bx

	mov ax,0
	mov es,ax
	mov bx,word ptr entry
//cuva staru rutinu

	mov ax,word ptr es:bx+2
	mov word ptr oldSEG,ax
	mov ax,word ptr es:bx
	mov word ptr oldOFF,ax
	
	//postavlja novu

	mov ax,word ptr newSEG
	mov word ptr es:bx+2,ax
	mov ax,word ptr newOFF
	mov word ptr es:bx,ax

	pop bx
	pop es
	pop ax
}
oldR=(IR)MK_FP(oldSEG,oldOFF);
unlock;
}

void IVTEntry::restoreINT() {
lock;
int entry=4*entryNo;
unsigned int tempSEG,tempOFF;

tempSEG=FP_SEG(*oldR);
tempOFF=FP_OFF(*oldR);

asm {
	push ax
	push es
	push bx

	mov ax,0
	mov es,ax
	mov bx,word ptr entry

	mov ax,word ptr tempSEG
	mov word ptr es:bx+2,ax
	mov ax,word ptr tempOFF
	mov word ptr es:bx,ax

	pop bx
	pop es
	pop ax
	
		mov al,20h
		out 20h,al
		
}
unlock;
}

void IVTEntry::add(IVTEntry* ivt) {
lock;
if(!firstIVT) {
	firstIVT=lastIVT=ivt;
}
lastIVT->nextIVT=ivt;
ivt->prevIVT=lastIVT;
lastIVT=lastIVT->nextIVT;
unlock;
}
//========================================================================
void IVTEntry::remove(IVTEntry* ivt) {
lock;
IVTEntry* old=firstIVT;
if(!old) return;

if(old==ivt) {
	if(!firstIVT->nextIVT) {  //ako je samo prvi u listi;
	firstIVT=lastIVT=0;
	unlock;
	return;
	}   else { 
firstIVT=firstIVT->nextIVT;
firstIVT->prevIVT=0;
unlock;
//brisacu ga u destruktoru
return;
	} 
}
	old=firstIVT->nextIVT;
	while(old!=ivt) old=old->nextIVT;
	if(old==lastIVT) {
	lastIVT=lastIVT->prevIVT;
	lastIVT->nextIVT=0;
	
	}else {
	old->prevIVT=ivt->nextIVT;
	old->nextIVT=ivt->prevIVT;
	}
unlock;
}
//=====================================================================
IVTEntry* IVTEntry::getIVT(int x) {
lock;
IVTEntry* tempIVT=firstIVT;
while(tempIVT) {
	if(tempIVT->entryNo==x) {
	
	break;
	}
	tempIVT=tempIVT->nextIVT;
}
unlock;
return tempIVT;
}
//====================================================================



