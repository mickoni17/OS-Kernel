/*
 * queue.cpp
 *
 *  Created on: May 28, 2018
 *      Author: Milorad Vulic
 */
#include "queue.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "system.h"
#include <stdio.h>
int syncPrintf(const char *format, ...);
#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }

Queue::Queue() {
	lock();
	first = last = 0;
	unlock();
}

Queue::~Queue() {
	while(first!=0){
		QNode *dlt=first;
		first=first->next;
		delete dlt;
	}
}

int Queue::empty() const {
	if (first !=0)
		return 0;
	else
		return 1;
}

void Queue::put(PCB* toBeBlocked){
	lock();
	QNode *newNode = new QNode();
	newNode->toBeBlocked = toBeBlocked;
	newNode->next = 0;

	//lock();
	if (first == 0)
		first = last = newNode;
	else {
		last->next = newNode;
		last = last->next;
	}
	unlock();
}

PCB* Queue::take(){
	if (first == 0) return 0;
	lock();
	QNode *dlt = first;
	PCB *taken = dlt->toBeBlocked;
	first = first->next;
	if (first == 0) last = 0;
	delete dlt;
	unlock();
	return taken;
}

SleepQueue::SleepQueue(){first=0;}

SleepQueue::~SleepQueue(){
	while(first!=0){
			SleepNode *dlt=first;
			first=first->next;
			delete dlt;
		}
}

void SleepQueue::dec(){
	//syncPrintf("Smanjujemmm mm\n");
	SleepNode* curr = first;
	lock();
	while(curr!=0 ){
		if(curr->time>0)curr->time--;
		curr=curr->next;
	}
		take();
	unlock();
}

void SleepQueue::put(PCB* toSleep,unsigned time){
	if(toSleep->getId()==0) return;
	if(time==0) return;
	SleepNode* newNode = new SleepNode();
	newNode->time=time;
	newNode->toSleep=toSleep;
	//printf("ubacen thread%d u sleep listu\n",toSleep->getId());
	lock();
	if(first==0){
	    first = newNode;
	    first->next=0;
	    unlock();
	    //syncPrintf("Ubacio %d u sleep\n",toSleep->getId());
	    return;
	}
	else{
		if(first->time>time){
			newNode->next=first;
			first=newNode;
	//		syncPrintf("Ubacio u sleep\n");
			unlock();
			return;
		}
		SleepNode* curr = first;
		while (curr->next!=0){
			if(curr->next->time > time) break;
			curr=curr->next;
		}
		if(curr->next==0){
			curr->next=newNode;
			newNode->next=0;
		}
		else{
			newNode->next=curr->next;
			curr->next=newNode;
		}
	}
	unlock();
    //syncPrintf("Ubacio u sleep\n");
    /*
	else{
		if(first->time>=time){
			newNode->next=first;
			newNode->time=time;
			SleepNode*curr=first;
			if(curr->time>time)
				curr->time=curr->time - time;
	else
			while(curr->time==time && curr!=0){
				curr->time=0;
				curr=curr->next;
			}
			first=newNode;
		}
		else{
			unsigned sum=first->time;
			SleepNode* curr=first;
			while(curr->next!=0){
				sum+=curr->next->time;
				if (sum>=time) break;
                curr=curr->next;
			}
			if(curr->next == 0){
				newNode->time=time-sum;
				newNode->next=0;
				curr->next=newNode;
			}
			else{
				 if(sum-time==curr->next->time)
					 newNode->time=0;
				 else
					 newNode->time=sum-time;
				newNode->next=curr->next;
				curr->next->time = curr->next->time - newNode->time;
				curr->next=newNode;
			}
		}
	}*/
}
void SleepQueue::take(){

	if(first!=0 && first->time<=0){
		while(first!=0 && first->time<=0){
			//printf("Uzimam thread%d iz sleep liste\n",first->toSleep->getId());
			SleepNode* dlt=first;
			first=first->next;
			dlt->toSleep->setState(READY);
			Scheduler::put(dlt->toSleep);
			delete dlt;
		}
	}
}
void SleepQueue::kolkoJeOstalo(){
	if(first!=0)
	syncPrintf("%d-%d",first->toSleep->getId(),first->time);
}

