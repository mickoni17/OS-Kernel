/*
 * kerSem.cpp
 *
 *  Created on: May 29, 2018
 *      Author: OS1
 */
#include "kerSem.h"
#include "queue.h"
#include "PCB.h"
#include "thread.h"
#include "system.h"
#include "SCHEDULE.h"

#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }

KernelSem::KernelSem(int v){
	lock();
	value=v;
	q=new Queue();
    unlock();
}

int KernelSem::val() const { return value; }

int KernelSem::wait(int toBlock){

	lock();
	if(toBlock!=0){
		if(value--<=0){
			PCB* curr=PCB::getRunning();
			curr->setState(BLOCKED);
			q->put(curr);
			unlock();
			dispatch();
			return 1;
		}
		else {
			unlock();
			return 0;
		}
	}
	else{
		if(value--<=0){
			unlock();
			return -1;
		}
		else{
			unlock();
			return 0;
		}
	}

}


void KernelSem::signal(){
	lock();
	if (value++<0){
		PCB* curr=q->take();
		if (curr==0) return;
		curr->setState(READY);
		Scheduler::put(curr);
	}
    unlock();
}
