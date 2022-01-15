#ifndef SYSTEM_CLASS
#define SYSTEM_CLASS
#include "pcb.h"
#include "thread.h"
#include "pcblist.h"
#include "idlethr.h"
#include "SemList.h"
class IVTEntry;
class System {
public:
	static PCB* mainPCB;
	static PCB* running;
	static PCBList* allPCB;
	static IdleThread* idle;
	static SemList* allSem;
	static int time;
	static int demandSwitch;
	static int lockFlag;
	static int lockSwitch;
	static IVTEntry* tabel[256];
};

#endif
