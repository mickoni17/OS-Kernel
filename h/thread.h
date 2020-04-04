/*
 * thread.h
 *
 *  Created on: May 26, 2018
 *      Author: Milorad Vulic
 */

#ifndef THREAD_H_
#define THREAD_H_
typedef unsigned int Time;
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096; //65536
const Time defaultTimeSlice = 2;

class PCB;
class Thread {
public:
     void start();
     void waitToComplete();
     virtual ~Thread();
     static void sleep(Time timeToSleep);
     static void wrapper();
     virtual void run() {}
protected:
     Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
private:
     PCB* myPCB;
public:
     PCB* getMyPCB(){
    	 return myPCB;
     }
};
void dispatch ();

#endif /* THREAD_H_ */
