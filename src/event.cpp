/*
 * event.cpp
 *
 *  Created on: Jul 8, 2018
 *      Author: OS1
 */

#include "event.h"
#include "KernelEv.h"
#include "system.h"
#include <stdio.h>

#define lock() {System::lok=0; asm pushf; asm cli;}
#define unlock() {System::lok=1; asm popf; }

Event::Event (IVTNo ivtNo){

	myImpl = new KernelEv(ivtNo);

}
Event::~Event (){

	delete myImpl;

}
void Event::wait (){

	myImpl->wait();

}
void Event::signal(){

	myImpl->signal();

}
