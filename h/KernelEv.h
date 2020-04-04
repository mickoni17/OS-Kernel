/*
 * KernelEv.h
 *
 *  Created on: Jul 8, 2018
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

typedef unsigned char IVTNo;
class PCB;

class KernelEv{
protected:
	friend class IVTEntry;
	PCB* myPCB;
	IVTNo ivtEntry;
	int value;
public:
	KernelEv(IVTNo);
	void wait();
	void signal();

};


#endif /* KERNELEV_H_ */
