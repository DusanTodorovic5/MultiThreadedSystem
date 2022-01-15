#include "pcb.h"
#include "pcblist.h"
#include "thread.h"
#include "dos.h"
#include "system.h"
#include "SCHEDULE.H"
#include "iostream.h"
int PCB::id = 0;
PCB::PCB(Thread* thread, unsigned stackSize, int timeSlice){
	this->unlockedByTime = 0;
	this->myThread = thread;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	state = ALOC;
	this->myId = ++id;
	stack = 0;
	this->ss = 0; this->sp = 0; this->bp = 0;
	this->ss_s = this->sp_s = this->bp_s = 0;
	this->isChild = 0;
	this->waitList = new PCBList();
	this->children = new PCBList();
	System::allPCB->insert(this);
}
void PCB::start(){

	stackSize /= sizeof(unsigned);

	stack = new unsigned[stackSize];


#ifndef BCC_BLOCK_IGNORE
	stack[stackSize-1] = 0x200;
	stack[stackSize-2] = FP_SEG((PCB::wrapper));
	stack[stackSize-3] = FP_OFF((PCB::wrapper));
	stack[stackSize - 12] = 0;
	this->ss = FP_SEG((stack + stackSize - 12));
	this->sp = FP_OFF((stack + stackSize - 12));
	this->bp = sp;

	this->bp_s = FP_OFF(stack + stackSize - 1);
	this->sp_s = this->bp_s;
	this->ss_s = ss;
#endif
	state = READY;
}
void PCB::insert(PCB* pcb){
	this->waitList->insert(pcb);
}
void PCB::remove(int id){
	this->waitList->remove(id);
}
void PCB::wrapper(){
	if (System::running->myThread){
		System::running->myThread->run();
		System::running->state = FINISHED;
	}
	System::lockFlag = 0;
	PCB* waited = 0;
	while ((waited = System::running->waitList->getFirst())){
		waited->state = READY;
		Scheduler::put(waited);
	}
	System::lockFlag = 1;

	dispatch();

}
PCB::~PCB(){
	delete stack;
	delete waitList;
	delete children;
}
