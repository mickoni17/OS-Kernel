/*
 * KernelEv.cpp
 *
 *  Created on: Jul 8, 2018
 *      Author: OS1
 */
#include "KernelEv.h"
#include "system.h"
#include "SCHEDULE.h"
#include "IVTEntry.h"
#include "thread.h"
#include "PCB.h"

#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }

KernelEv::KernelEv(IVTNo no){
	lock();
	value=0;
	myPCB=PCB::getRunning();
	ivtEntry=no;
	IVTEntry::allIvt[ivtEntry]->setEvent(this);
	unlock();
}
void KernelEv::wait(){
	if(myPCB!=PCB::getRunning())
		return;
	lock();
	if (value==1){
		value=0;
		unlock();}
	else{
		value=-1;
		myPCB->setState(BLOCKED);
		unlock();
		dispatch();
	}

}
void KernelEv::signal(){
	lock();
	if (value>=0)
		value=1;
	else{
		value=0;
		myPCB->setState(READY);
		Scheduler::put(myPCB);
	}
	unlock();


}

