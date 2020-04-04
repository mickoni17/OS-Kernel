/*
 * PCB.h
 *
 *  Created on: May 27, 2018
 *      Author: Milorad Vulic
 */

#ifndef PCB_H_
#define PCB_H_
#include "thread.h"
//typedef unsigned int Time;
//typedef unsigned long StackSize;
typedef int ID;
enum ThreadStates {READY, BLOCKED, SLEEP, NEW, DONE};
class Queue;

class PCB{
private:
	friend class Thread;
	friend class Idle;
    static PCB* running;
	static ID nextID;
	ID id;
	Time exec,sleep;
	unsigned sp,ss,size,*stack;
	Thread* myThread;
	ThreadStates state;
    Queue* blockedList;
public:
    PCB(Thread*, StackSize, Time);
    void waitToComplete();
    void createStack();
    static PCB* getRunning(){
    	return running;
    }
    Time getExecTime(){
    	return exec;
    }
	const Queue* getBlockedList() const {
		return blockedList;
	}
	void setBlockedList(Queue* blockedList) {
		this->blockedList = blockedList;
	}
	unsigned getSp() const {
		return sp;
	}
	void setSp(unsigned sp) {
		this->sp = sp;
	}
	unsigned getSs() const {
		return ss;
	}
	void setSs(unsigned ss) {
		this->ss = ss;
	}
	ThreadStates getState() const {
		return state;
	}
	void setState(ThreadStates state) {
		this->state = state;
	}
	ID getId() const {
		return id;
	}
	static void setRunning(PCB* ru) {
		running = ru;
	}
	Thread* getMyThread() const {
		return myThread;
	}
	void setMyThread(Thread* myThread) {
		this->myThread = myThread;
	}
	void run(){
		myThread->run();
	}
};

#endif /* PCB_H_ */
