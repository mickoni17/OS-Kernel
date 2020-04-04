/*
 * system.h
 *
 *  Created on: Jul 3, 2018
 *      Author: OS1
 */

#ifndef PROJEKAT_SYSTEM_H_
#define PROJEKAT_SYSTEM_H_

typedef void interrupt(*interr)(...);
class Idle;
class PCB;
class SleepQueue;
class System{
    static Idle *idle;
    static interr old;
    static PCB* mainPCB;
public:
    System();
	static void interrupt timer(...);
	static void dispatch();
	static void init();
	static void restore();
	static int lok;
	static volatile int requestedContextSwitch;
	volatile static int counter;
	static SleepQueue* sleeping;
};



#endif /* PROJEKAT_SYSTEM_H_ */
