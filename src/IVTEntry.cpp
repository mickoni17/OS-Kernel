/*
 * IVTEntry.cpp
 *
 *  Created on: Jul 8, 2018
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "system.h"
#include "KernelEv.h"
#include <dos.h>
#include <stdio.h>

#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }
IVTEntry* IVTEntry::allIvt[256];

IVTEntry::IVTEntry(IVTNo entry,interr newInt){
	lock();
	oldInt=0;
	this->newInt=newInt;
	this->entry=entry;
	myEvent=0;
#ifndef BCC_BLOCK_IGNORE
	oldInt=getvect(entry);
	setvect(entry, newInt);
#endif
	allIvt[entry]=this;
	unlock();
}
IVTEntry::~IVTEntry(){
	lock();
	(*oldInt)();
#ifndef BCC_BLOCK_IGNORE
	setvect(entry, oldInt);
#endif
	allIvt[entry]=0;
	unlock();
}
void IVTEntry::oldInterrupt(){
	(*oldInt)();
}
void IVTEntry::signal(){
	if(myEvent==0)
		return;
	myEvent->signal();
}
void IVTEntry::removeEvent(){
	lock();
	myEvent=0;
	unlock();
}
void IVTEntry::setEvent(KernelEv* kEvent){
	lock();
	myEvent=kEvent;
	unlock();
}
