#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time;
const Time defaultTimeSlice = 2;
typedef int ID;
typedef char* TName;

class PCB;

class Thread  {
public:
ID start();    //returns : thread   ID
static int waitToComplete(ID id); // returns: 1 - run completed,
									//0 - woken up, -1  -  error - no thread with given ID
static int sleep(Time timeToSleep);   //returns  1 - sleep completed,
									//  0 - woken up
static int wakeUp(ID id);   // returns:  1 - deblocked, 0 - no effect,
							// -1 - error - no thread with given ID



ID getId();
TName getName();
static int waitForChildren();   // ovo sam ja izmajmunisao ali i dalje u originalu nije zahtevano ovako :S S:
static Thread* getThreadById(ID id);
static ID getIdOf(TName name);
static TName getName(ID id);
virtual ~Thread();

protected:
//friend class userMainT;
friend int main(int argc,char* argv[]);
friend class IThread;
friend class PCB;
friend class System;
Thread(TName name,StackSize stackSize=defaultStackSize,Time timeSlice=defaultTimeSlice);
virtual void run() {}
//int waitForChildren();  //return: 1 - finished, 0 - woken up
int waitToComplete();    //return: 1 - completed,  0 - woken up

private:

PCB* myPCB;

};   //end thread 
void dispatch();

#endif