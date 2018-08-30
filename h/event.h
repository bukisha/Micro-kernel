// File: event.h 
#ifndef _event_h_ 
#define _event_h_ 
#include "IVTE.h"


typedef unsigned char IVTNo; 
class KernelEv; 
 
class Event { 
friend class Thread;
friend class KernelEv;
public: 

   Event (IVTNo ivtNo); 
  ~Event (); 
 
 
   int wait(); 
   void signal(); 
 
private: 
   KernelEv* myImpl; 
}; 
#endif