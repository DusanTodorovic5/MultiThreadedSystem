#include "idlethr.h"
#include "pcb.h"
#include "thread.h"
IdleThread::IdleThread() : Thread(4096,1){ }
void IdleThread::run(){
	int i =0;
	for (i=0;i<20;i++){
		if (i == 10){
			i = 0;
		}
	}
}
void IdleThread::start(){
	myPCB->start();
}
PCB* IdleThread::getPCB(){
	return myPCB;
}
