#ifndef _Lista_h_
#define _Lista_h_
#include "Thread.h"
#include <iostream.h>
#include "stdio.h"
#include "IVTE.h"
class PCB;


class Element {
	
public:
	
	Element(PCB* p) {
	pcb=p;
	next=0;
	prev=0;
	}
	~Element() {}

	PCB* pcb;
	Element* next;
	Element* prev;
};

class Lista   {
friend class System;
friend class IVTEntry;
friend class PCB;
friend class Thread;
friend int userMain();
friend class UserT1;//za potrebe testiranja
public:
	Lista();
	~Lista();
	void put(PCB* pcb);
	PCB* get();     //vraca prvi pcb iz liste
	void remove(ID id);  //izbacuje iz liste pcb sa zadatim ID-em
	void get(int tempID);  
	int size() const ;   //vraca broj elemenata u listi
	void writeList();
private:
Element* first;
Element* last;
int len;   //broj elemenata u listi
};

#endif