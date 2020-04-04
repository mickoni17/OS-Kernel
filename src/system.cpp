/*
 * system.cpp
 *
 *  Created on: Jul 3, 2018
 *      Author: OS1
 */

#include "system.h"
#include "PCB.h"
#include <dos.h>
#include "SCHEDULE.h"
#include "thread.h"
#include "idle.h"
#include "queue.h"
#include <stdio.h>

int syncPrintf(const char *format, ...);

unsigned tsp;
unsigned tss;
Idle* System::idle=0;
interr System::old=0;
int System::lok=1;
PCB* System::mainPCB=0;
SleepQueue* System::sleeping=new SleepQueue();
volatile int System::counter=0;
int volatile System::requestedContextSwitch=0;
#define lock() {lok=0; asm pushf; asm cli;}
#define unlock() {lok=1; asm popf; }

extern void tick();


void interrupt System::timer(...){

	if (requestedContextSwitch==0 && counter>0){
		tick();
		counter--;
		(*old)();
		sleeping->dec();
	}
	else
		counter=0;

	if (counter == 0 || requestedContextSwitch==1) {
			if(lok){
                lok=0;
				requestedContextSwitch=0;

				asm {
					mov tsp, sp
					mov tss, ss
				}

				PCB::getRunning()->setSp(tsp);
				PCB::getRunning()->setSs(tss);

				if(PCB::getRunning()->getState()==READY && PCB::getRunning()->getId()!=0)
					Scheduler::put(PCB::getRunning());
				PCB::setRunning(Scheduler::get());

				if (PCB::getRunning()==0)
					PCB::setRunning(idle->getMyPCB());
				//syncPrintf("Running je %d\n",PCB::getRunning()->getId());
	/*!!!!!!*/	lok=1;
				tsp = PCB::getRunning()->getSp();
				tss = PCB::getRunning()->getSs();

				counter = PCB::getRunning()->getExecTime();

				asm {
					mov sp, tsp   // restore sp
					mov ss, tss
				}
				lok=1;
			}
			else
				requestedContextSwitch = 1;
	}

	/*if(!requestedContextSwitch) asm int 60h;
	requestedContextSwitch = 0;*/
}

void System::dispatch(){
	requestedContextSwitch=1;
	timer();
}

void System::init(){
    idle = new Idle();
    idle->start();
    mainPCB = new PCB(0,65534,2);
    counter=mainPCB->getExecTime();
    mainPCB->setState(READY);
    PCB::setRunning(mainPCB);

#ifndef BCC_BLOCK_IGNORE
	asm cli;
	old=getvect(0x08);
	setvect(0x08, timer);
	asm sti;
#endif

}

void System::restore(){
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x08, old);
	asm sti;
#endif
}
