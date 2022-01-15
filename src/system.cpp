#include "system.h"
#include "dos.h"
#include "thread.h"
#include "pcb.h"
#include "pcblist.h"
#include "SCHEDULE.H"
#include "ivtentry.h"
PCB* System::mainPCB = 0;
PCB* System::running = 0;
PCBList* System::allPCB = 0;
IdleThread* System::idle = 0;
int System::time = 20;
int System::demandSwitch = 0;
int System::lockFlag = 1;
int System::lockSwitch = 0;
SemList* System::allSem = 0;
IVTEntry* System::tabel[256] = {0};
extern void tick();
//void tick(){}
unsigned tsp = 0;
unsigned tss = 0;
unsigned tbp = 0;
void interrupt timer(){
		if (!System::demandSwitch)
				System::time--;
		if (System::time == 0 || System::demandSwitch || System::lockSwitch){
			if (System::lockFlag){
				System::lockSwitch = 0;
		#ifndef BCC_BLOCK_IGNORE
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
		#endif

				System::running->sp = tsp;
				System::running->ss = tss;
				System::running->bp = tbp;
				if (System::running->state == READY && System::running->myId != System::idle->getId()){
						Scheduler::put(System::running);
				}
				System::running = Scheduler::get();
				if (System::running == 0){
					System::running = System::idle->getPCB();
				}
				tsp = System::running->sp;
				tss = System::running->ss;
				tbp = System::running->bp;
				System::time = System::running->timeSlice;
		#ifndef BCC_BLOCK_IGNORE
				asm {
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
				}
		#endif
			}
			else {
				System::lockSwitch = 1;
			}
		}
		if (!System::demandSwitch){
			System::allSem->decAll();
			tick();
#ifndef BCC_BLOCK_IGNORE
			asm int 60h
#endif
		}
		System::demandSwitch = 0;
}

unsigned oldTimerOFF = 0, oldTimerSEG = 0;
void inic(){
	asm{
			cli
			push es
			push ax


			mov ax, 0
			mov es, ax


			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax


			mov ax, seg timer
			mov word ptr es:0022h, ax
			mov ax, offset timer
			mov word ptr es:0020h, ax

			mov ax, oldTimerSEG
			mov word ptr es:0182h, ax
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax


			pop ax
			pop es
			sti
		}
}
void restore(){
	if (System::running != System::mainPCB)
		return;
	asm {
				cli
				push es
				push ax

				mov ax,0
				mov es,ax
				mov ax, word ptr oldTimerSEG
				mov word ptr es:0022h, ax
				mov ax, word ptr oldTimerOFF
				mov word ptr es:0020h, ax

				pop ax
				pop es
				sti
			}
}

