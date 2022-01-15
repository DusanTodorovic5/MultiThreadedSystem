#include "ivtentry.h"
#include "system.h"
#include "krnlEv.h"
#include "dos.h"
IVTEntry::IVTEntry(unsigned char ivtNo, pInterrupt newR){
	System::lockFlag = 0;
	this->event = 0;
	this->ivtNo = ivtNo;
	System::tabel[ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	old = getvect(ivtNo);
	setvect(ivtNo, newR);
#endif
	System::lockFlag = 1;
}
IVTEntry::~IVTEntry(){
	oldRoutine();
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, old);
#endif
}
void IVTEntry::signal(){
	if (event != 0)
		event->signal();
}
void IVTEntry::oldRoutine(){
	System::lockFlag = 0;
	if (old != 0){
		(*old)();
	}
	System::lockFlag = 1;
}
