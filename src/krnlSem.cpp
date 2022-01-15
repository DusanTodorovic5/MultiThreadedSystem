#include "krnlSem.h"
#include "system.h"
#include "SCHEDULE.H"
#include "waitlist.h"
int KernelSem::_id = 0;
KernelSem::KernelSem(int init){
	this->id = ++_id;
	blockedList = new WaitList();
	value = init;
	System::allSem->insert(this);
}
KernelSem::~KernelSem(){
	delete blockedList;
	System::allSem->remove(this->id);
}
int KernelSem::wait(unsigned int maxTimeToWait){
	System::lockFlag = 0;
	this->value--;
	if (this->value < 0){
		System::running->state = BLOCKED;
		blockedList->insert(System::running,maxTimeToWait);

		System::lockFlag = 1;
		dispatch();
		System::lockFlag = 0;
	}
	if (System::running->unlockedByTime == 1){
		System::running->unlockedByTime = 0;
		System::lockFlag = 1;
		return 0;
	}
	System::lockFlag = 1;
	return 1;
}
void KernelSem::signal(){
	System::lockFlag = 0;
	this->value++;
	PCB* unblocked = blockedList->removeFirst();
	if (unblocked != 0){
		unblocked->state = READY;
		Scheduler::put(unblocked);
	}
	System::lockFlag = 1;
}
void KernelSem::decrement(){
	WaitList::WaitElem* curr = blockedList->getHead();
	while (curr != 0){
		WaitList::WaitElem* we = curr;
		curr = curr->next;
		we->time--;
		if (we->time == 0){
			PCB* p = blockedList->remove(we->data->myId);
			p->unlockedByTime = 1;
			p->state = READY;
			Scheduler::put(p);
		}
	}
}
int KernelSem::val() const{
	return value;
}
