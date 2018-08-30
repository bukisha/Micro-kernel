#include "Semaphor.h"
#include "lock.h"
#include "Ksem.h"

Semaphore::Semaphore(int init)  {
lock;
myImpl=new KernelSem(init,this);
unlock;
}

Semaphore::~Semaphore()  {
lock;
delete myImpl;
unlock;

}

int Semaphore::wait() {
lock;
int ret;
ret=myImpl->wait();
unlock;
return ret;
}

void Semaphore::signal() {
lock;
myImpl->signal();
unlock;
}

int Semaphore::val() const {

return myImpl->value;

}