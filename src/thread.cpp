#include "thread.h"
#include "pcb.h"
#include "pcblist.h"
#include "SCHEDULE.H"
#include "dos.h"
#include "system.h"
Thread::Thread(StackSize stackSize, Time timeSlice){
	System::lockFlag = 0;
	myPCB = new PCB(this,stackSize,timeSlice);
	System::lockFlag = 1;
}
void Thread::start(){
	System::lockFlag = 0;
	myPCB->start();
	Scheduler::put(myPCB);
	System::lockFlag = 1;
}
void Thread::waitToComplete(){
	System::lockFlag = 0;
	if (this->myPCB == System::running || this->myPCB->state == FINISHED || System::running->state == FINISHED){
		System::lockFlag = 1;
		return;
	}
	System::running->state = BLOCKED;
	myPCB->waitList->insert(System::running);
	System::lockFlag = 1;
	dispatch();
}
ID Thread::getId(){
	return myPCB->myId;
}
ID Thread::getRunningId(){
	return System::running->myId;
}
Thread* Thread::getThreadById(int id){
	PCB* p = System::allPCB->get(id);
	return p->myThread;
}
PCB* Thread::getPCB(){
	return myPCB;
}
Thread::~Thread(){
	waitToComplete();
	System::allPCB->remove(myPCB->id);
	delete myPCB;
}
void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	asm pushf
	asm cli
#endif
	System::demandSwitch = 1;
#ifndef BCC_BLOCK_IGNORE
	asm int 8h
	asm popf
#endif
}
int ret_val = 0;
PCB* child;
unsigned ss_res, bp_res;
unsigned poff,coff;
unsigned *oldBp, *newBp;
void interrupt fork_int(){
	ret_val = 0;
	child = 0;
	if (System::running != System::mainPCB){
		Thread* t = System::running->myThread->clone();
		if (t == 0){
			ret_val = -1;
			return;
		}
		child = t->getPCB();
	}
	else{
		ret_val = -1;
		return;
	}
	if (child == 0){
		ret_val = -1;
		return;
	}
	child->timeSlice = System::running->timeSlice;
	child->stackSize = System::running->stackSize;
	child->stack = new unsigned[System::running->stackSize];
	if (child->stack == 0){
		delete child;
		ret_val = -1;
		return;
	}
	child->stackSize = System::running->stackSize;
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov ss_res, ss;
		mov bp_res, bp;
	}
	int dif = (unsigned*)MK_FP(System::running->ss_s, System::running->bp_s) - (unsigned*)MK_FP(ss_res,bp_res);
	child->bp = FP_OFF(child->stack + child->stackSize - 1 - dif);
	child->bp_s = FP_OFF(child->stack + child->stackSize - 1);
	child->ss_s = FP_SEG(child->stack + child->stackSize - 1);
	child->sp_s = child->bp_s;
	child->ss = child->ss_s;
	child->sp = child->bp;
	for (int i=0;i<(dif+1)*2;i++){
		*(unsigned*)MK_FP(child->ss,child->bp + i) = *(unsigned*)MK_FP(ss_res,bp_res + i);
	}

	poff = bp_res;
	coff = child->sp;
	int t = 1;
	while (t){
		oldBp = (unsigned*)MK_FP(ss_res,poff);
		newBp = (unsigned*)MK_FP(child->ss,coff);
		if (*oldBp == 0){
			*newBp = 0;
			t = 0;
			break;
		}
		*newBp = *oldBp - poff + coff;
		poff = *oldBp;
		coff = *newBp;
	}
#endif
	child->isChild = 1;
	child->state = READY;
	System::running->children->insert(child);
	Scheduler::put(child);
	ret_val = child->myId;
}
ID Thread::fork(){
	System::lockFlag = 0;
	fork_int();
	int retVal = ret_val;
	PCB* ch = child;
	System::lockFlag = 1;
	if (retVal == -1){
		return -1;
	}
	if (System::running == ch){
		return 0;
	}
	else{
		return ch->myId;
	}
}
void Thread::exit(){
	System::lockFlag = 0;
	PCB* waited = 0;
	while ((waited = System::running->waitList->getFirst())){
		waited->state = READY;
		Scheduler::put(waited);
	}
	System::lockFlag = 1;

	dispatch();
}
void Thread::waitForForkChildren(){
	System::running->children->waitAll();
}
Thread* Thread::clone() const{
	return new Thread(this->myPCB->stackSize*sizeof(this->myPCB->stack),this->myPCB->timeSlice);
}
