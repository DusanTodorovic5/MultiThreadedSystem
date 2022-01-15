#ifndef IDLE_THREAD_CLASS
#define IDLE_THREAD_CLASS
#include "thread.h"

class IdleThread : public Thread{
public:
	void run();
	void start();
	IdleThread();
	PCB* getPCB();
};

#endif
