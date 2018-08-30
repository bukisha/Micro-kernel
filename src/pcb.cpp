#include "pcb.h"
#include "thread.h"
#include "dos.h"
#include "stdlib.h"
#include "system.h"
#include "string.h"
#include<iostream.h>
#include "stdio.h"
#include "lock.h"
#include "conio.h"
const int PCB::NEW=1;
const int PCB::READY=2;
const int PCB::BLOCKED=3;
const int PCB::OVER=4;
const int PCB::WAITCHILDREN=5;
ID PCB::globalID=0;

Lista* PCB::ThreadPool=new Lista();

PCB::PCB(TName name,StackSize s,Time t,Thread* tt) {   //konstruktor nezavrsen bez semafora i eventova ik o zna cega jos :D
	lock;
//cprintf("\nusao u PCB konstruktor");
	myID=++globalID;
	ThreadPool->put(this);
	unsigned int TS=t*55;
	this->myTimeSlice=TS;
	this->myThread=tt;
	if(name) {
	this->myName=new char[10];
	strcpy(myName,name);
	} else {
	this->myName=new char[10];
	strcpy(this->myName,"Thread");
	itoa(this->myID, (this->myName + 6) ,10);
	}
	this->sleeping=0;
	this->sleepCNT=0;
	this->awaken=1;
//	compleated=1;
	waitLista=new Lista();      //lista niti koje cekaju da se ova
								//nit zavrsi pa da mogu da se utepaju :D
	if(System::running ) {//&& myID!=2) {
		myParent=System::running->myThread;
		System::running->numOfChildren++;
	
	} else myParent=0;
	numOfChildren=0;
	//waitReturn=1;
	
	this->myStackSize=s;
	this->myPassedTime=0;
	this->blockedOnThread=0;
	this->mySem=0;
	this->myEvent=0;
	state=NEW;
	myStack=0;   //stek pravim kad se pozove start a ne sad odma :D
	mySS=0;
	myBP=0;
	mySP=0;
unlock;
}

PCB::~PCB() {  //destruktor isto polovican zavrsi ga kad napises semafore
lock;
//ThreadPool->remove(this->myID);
delete waitLista;
delete  myStack;
delete [] myName;
unlock;
}

void PCB::buildStack()  {
lock;
	//cprintf("\nNAPRAVIO sam Stek");
	static volatile unsigned newCS,newPC,newSS,newSP,oldSS,oldSP,oldBP,newBP;
	static unsigned *tempStack;
	tempStack=new unsigned[myStackSize];
	myStack=tempStack;

	newCS=FP_SEG(&(System::wrapper));
	
	newPC=FP_OFF(&(System::wrapper));
	
	newSS=mySS=FP_SEG(tempStack+myStackSize);
	
	newSP=mySP=FP_OFF(tempStack+myStackSize);


asm {
	 mov oldSS,ss      
	 mov oldSP,sp
	 mov oldBP,bp

	 mov ss,newSS
	 mov sp,newSP
	 
	 push ax
	 pushf               
	 pop ax
	 or ax,0000001000000000b 
	 push ax
	 mov ax,newCS
	 push ax
	 mov ax,newPC
	 push ax
	
	 mov ax,0

	push ax				
	push bx				
	push cx					
	push dx
	push es
	push ds
	push si
	push di
	push bp
	
	mov newSP,sp   
	
	mov newSS,ss
	mov sp,oldSP	
	mov ss,oldSS
	mov bp,oldBP	
	
}
mySP=newSP;
mySS=newSS;
myBP=newSP;
unlock;
}


