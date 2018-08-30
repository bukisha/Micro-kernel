#include "lock.h"
#include "IThread.h"
#include "PCB.h"
#include "System.h"
#include <iostream.h>
#include<stdio.h>
IThread::IThread() : Thread("idleThread",256,1) {}

void IThread::run() {
	while(1);
/*
{
	lock;
	cout<<"IDLE!!!!"<<endl;
	unlock;
	}*/
}

void IThread::start() {
lock;
myPCB->state=PCB::READY;
myPCB->buildStack();

unlock;
}