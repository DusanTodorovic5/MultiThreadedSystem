#include "krnlEv.h"
#include "system.h"
#include "thread.h"
#include "ivtentry.h"
#include "SCHEDULE.H"
KernelEv::KernelEv(unsigned char ivtNo){
	System::lockFlag = 0;
	this->ivtNo = ivtNo;
	System::tabel[ivtNo]->event = this;
	kreator = System::running;
	value = 1;
	System::lockFlag = 1;
}
KernelEv::~KernelEv(){

}
void KernelEv::signal(){
	System::lockFlag = 0;
	if (this->value == 0){
		this->value = 1;
		this->kreator->state = READY;
		Scheduler::put(this->kreator);
		System::lockFlag = 1;
		dispatch();
		System::lockFlag = 0;
	}

	System::lockFlag = 1;
}
void KernelEv::wait(){
	System::lockFlag = 0;
	if (System::running == this->kreator){
		if (this->value == 1){
			System::running->state = BLOCKED;
			System::lockFlag = 1;
			this->value = 0;
			dispatch();
			System::lockFlag = 0;
		}
	}
	System::lockFlag = 1;
}
