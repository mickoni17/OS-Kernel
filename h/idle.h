/*
 * idle.h
 *
 *  Created on: Jun 29, 2018
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_
#include "system.h";
#include "queue.h";

class Thread;
int syncPrintf(const char *format, ...);
class Idle : public Thread{
protected:
	volatile int i,j;
	void run(){
		while(i){
			//for(j=0;j<500000;j++)
			//	if(j==1)
			//System::sleeping->kolkoJeOstalo();
			//		syncPrintf("idle");


		}

	}
public:
	Idle(StackSize ss = defaultStackSize, Time t = defaultTimeSlice):Thread(ss,t){i=1;j=0;};
};

#endif /* IDLE_H_ */
