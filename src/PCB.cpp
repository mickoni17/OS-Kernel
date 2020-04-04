/*
 * PCB.cpp
 *
 *  Created on: May 30, 2018
 *      Author: OS1
 */

#include "PCB.h"
#include "SCHEDULE.H"
#include "dos.h"
#include "thread.h"
#include "queue.h"
#include "system.h"
#include <stdio.h>

ID PCB::nextID = 0;
PCB* PCB::running=0;
#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }

int syncPrintf(const char *format, ...);
PCB::PCB(Thread *thread,StackSize sSize = defaultStackSize, Time timeSlice = defaultTimeSlice){
	lock();
	if (sSize>defaultStackSize) sSize = defaultStackSize;
	size = sSize/sizeof(unsigned)-1;
	exec=timeSlice;
	myThread=thread;
	id=nextID++;
	//syncPrintf("Napravio se thread %d\n",id);
	state=NEW;
	ss=0;
	sp=0;
	sleep=0;
	blockedList=new Queue();
	stack=0;
    unlock();
}

void PCB::waitToComplete(){
    lock();
	if (state==DONE || myThread==running || state==NEW || id==0){
		unlock();
		return;
	}
    	running->state = BLOCKED;
	    blockedList->put(running);
	unlock();
	//printf("%d se blokira na %d\n",running->getId(),getId());
	dispatch();
}
void PCB::createStack(){

	stack=new unsigned[size];

	stack[size-1]=0x200;
	#ifndef BCC_BLOCK_IGNORE
	stack[size-2]=FP_SEG(&Thread::wrapper);
	stack[size-3]=FP_OFF(&Thread::wrapper);
	sp=FP_OFF(stack+size-12);
	ss=FP_SEG(stack+size-12);
	#endif

	state=READY;
	if (id!=0)
		Scheduler::put(this);

}
