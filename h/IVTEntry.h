/*
 * IVTEntry.h
 *
 *  Created on: May 26, 2018
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
newEntry##numEntry.signal();\
if (callOld == 1)\
newEntry##numEntry.oldInterrupt();\
}

typedef unsigned char IVTNo;
typedef void interrupt(*interr)(...);
class KernelEv;

class IVTEntry{
private:
	IVTNo entry;
	interr oldInt,newInt;
    KernelEv* myEvent;
public:
    static IVTEntry *allIvt[];

    IVTEntry(IVTNo entry,interr newInt);
    ~IVTEntry();
    void oldInterrupt();
    void signal();
    void removeEvent();
    void setEvent(KernelEv* kEvent);
};

#endif /* IVTENTRY_H_ */
