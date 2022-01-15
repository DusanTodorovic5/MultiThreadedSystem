// pcb.h
#ifndef PCB_CLASS
#define PCB_CLASS

#include "thread.h"
class PCBList;
enum State { ALOC = 0,READY,BLOCKED, RUNNING, FINISHED };
class PCB{
public:
	PCB(Thread*,unsigned, int);
	~PCB();
	unsigned ss;
	unsigned ss_s;
	unsigned sp;
	unsigned sp_s;
	unsigned bp;
	unsigned bp_s;
	int isChild;
	int timeSlice;

	unsigned *stack;
	unsigned stackSize;

	State state;

	int myId;
	static int id;

	Thread* myThread;
	PCBList* waitList;
	PCBList* children;

	int unlockedByTime;

	void start();
	void insert(PCB*);
	void remove(int);

	static void wrapper();

};

#endif
