/*
 * thread.cpp
 *
 *  Created on: May 30, 2018
 *      Author: OS1
 */
#include "thread.h"
#include "SCHEDULE.h"
#include "system.h"
#include "PCB.h"
#include "queue.h"
#include "idle.h"
#include <stdio.h>

#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }
int syncPrintf(const char *format, ...);
Thread::Thread (StackSize stackSize, Time timeSlice) {
   	lock();
    myPCB = new PCB(this,stackSize,timeSlice);
    unlock();
}

Thread::~Thread(){
	waitToComplete();
	lock();
	delete myPCB;
	unlock();
}


void Thread::waitToComplete(){
	myPCB->waitToComplete();

}

void Thread::start(){
	lock();
	myPCB->createStack();
	//syncPrintf("Pocinjem thread %d\n",myPCB->getId());
	unlock();
}

void Thread::wrapper(){
	PCB::getRunning()->run();
	lock();
	PCB::getRunning()->state = DONE;
    //syncPrintf("thread%d gotov\n",PCB::getRunning()->getId());
    PCB* curr=PCB::getRunning()->blockedList->take();
    if (curr!=0){
		curr->state=READY;
		Scheduler::put(curr);
		while(curr!=0){
			curr=PCB::getRunning()->blockedList->take();
			if(curr==0) break;
			curr->state=READY;
			Scheduler::put(curr);
		}
    }
    unlock();
    dispatch();
}
void Thread::sleep(Time timeToSleep){
	//lock();
	//syncPrintf("Spavam\n");
	if(PCB::getRunning()->getId()==0) return;
	PCB* curr=PCB::getRunning();
	curr->setState(SLEEP);
	System::sleeping->put(curr,timeToSleep);
	//unlock();
	dispatch();

}
void dispatch(){
    System::dispatch();

}

