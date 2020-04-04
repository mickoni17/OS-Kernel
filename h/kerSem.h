/*
 * kerSem.h
 *
 *  Created on: May 29, 2018
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

class Queue;

class KernelSem {
private:
	int value;
	Queue* q;
public:
	KernelSem(int v);
	int wait(int toBlock);
	void signal();
	int val() const;
};

#endif /* KERSEM_H_ */
