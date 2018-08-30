#ifndef _semaphor_h_ 
#define _semaphor_h_ 
extern int semPreempt; 
 
class KernelSem; 
 
class Semaphore { 
  friend class Thread;

public: 
  
  Semaphore (int init=1); 
 ~Semaphore (); 
 
  virtual int wait(); 
  virtual void signal(); 
 
  int val () const;  // Returns the current value of the semaphore 
 
private: 
  KernelSem* myImpl; 
}; 
#endif