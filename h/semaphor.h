/*
 * semaphor.h
 *
 *  Created on: May 26, 2018
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

class KernelSem;

class Semaphore {
private:
 KernelSem* myImpl;
public:
 Semaphore (int init=1);
 virtual ~Semaphore ();

 virtual int wait (int toBlock);
 virtual void signal();

 int val() const; // Returns the current value of the semaphore
};

#endif /* SEMAPHOR_H_ */
