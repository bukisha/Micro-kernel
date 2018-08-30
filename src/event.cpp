#include "event.h"
#include "IVTE.h"
#include "lock.h"
#include "KernEv.h"

Event::Event(IVTNo no) { 
lock;
myImpl=new KernelEv(no);
unlock;
}

Event::~Event() {
lock;
delete myImpl;
unlock;

}

int Event::wait() {
lock;
int ret=myImpl->wait();
unlock;
return ret;
}

void Event::signal() {
lock;
myImpl->signal();
unlock;
}