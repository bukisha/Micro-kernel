#include "Lista.h"
#include "PCB.h"
#include "System.h"
#include "lock.h"


Lista::Lista()  {
	lock;  
	first=last=0;
   len=0;
	unlock;
}

Lista::~Lista()  {
	lock;
	Element* temp;
	while(first!=0) 
	{
	  temp=first;   
	  first=first->next;
	  delete temp;
	  len--;
	}
	unlock;
}

void Lista::put(PCB* p) {
	lock;
	Element* temp=new Element(p);
	if(!first) 
	{
		first=temp;
		last=temp;
		
	} else 
		{
			last->next=temp;
			temp->prev=last;
			last=last->next;
		}
	len++;
unlock;
}



PCB* Lista::get()  {
    lock;
	Element* temp=first;
	PCB* old;
	
	if(!first) return 0;
				if(!(first->next))  
				{
					temp=first;
					old=temp->pcb;   //obrati paznju ovde si drkao sa brisanjem tempa
					first=last=0;
				} else 
						{
						      temp=first;
							  old=temp->pcb;
							  first=first->next;
							  first->prev=0;
						}
				delete temp;
				len--;
				unlock;
				return old;
}				

int Lista::size() const {
    
	return len;
	
}

void Lista::remove(ID id) {
	lock;
Element* temp=first;
Element* old;
while(temp->pcb->myID!=id) {
temp=temp->next;
}
if(temp==first) {
	old=temp;
	first=first->next;
	first->prev=0;
	len--;
	delete old;
	unlock;
	return;
}
if(temp!=first && temp!=last) {
old=temp;
	Element* p=temp->prev;
Element* s=temp->next;
p->next=s;
s->prev=p;
len--;
delete old;
unlock;
return;
}
if(temp==last) {
	old=temp;
last=last->prev;
last->next=0;
len--;
delete old;
unlock;
return;
}
	
}//end remove


void Lista::writeList() {    
   lock;
		Element* t=first;
		//printf("\nPOCETAK");
		//cout<<"\n"<<t->pcb->myThread->getName()<<endl;
		//printf("\nKRAJ");
		if(t==0){ 
			printf("\nLista prazna");
			return;
		}
		printf("\nPOCETAK LISTE");
		while(t!=0) {
			 
			cout<<"\n"<<t->pcb->myThread->getName()<<t->pcb->myThread->getId()<<"["<<t->pcb->sleepCNT<<"]"<<endl;
		
			t=t->next;
		}
		printf("\nVelicina liste je: %d",size());
		printf("\nKRAJ LISTE");
		
		unlock;
}