/*
 * queue.h
 *
 *  Created on: May 26, 2018
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_

class PCB;
struct QNode{
	PCB* toBeBlocked;
	QNode* next;
};

class Queue {
private:
	QNode *first,*last;
public:
	Queue();
    ~Queue();
    int empty() const;
    void put(PCB* toBeBlocked);
    PCB* take();


};

struct SleepNode{
	PCB* toSleep;
	unsigned time;
	SleepNode* next;
};

class SleepQueue {
private:
	SleepNode *first;
public:
	SleepQueue();
    ~SleepQueue();
    int empty() const;
    void dec();
    void put(PCB* toBeBlocked,unsigned time);
    void take();
    void kolkoJeOstalo();
};

/*class KernelEv;
struct EventNode{
	KernelEv* kEvent;
	EventNode* next;
};

class EventQueue {
private:
	EventNode *first,*last;
public:
	EventQueue();
    ~EventQueue();
    void signal();
    void put(KernelEv* kEvent);
    void take(KernelEv* kEvent);
};*/

#endif /* QUEUE_H_ */
