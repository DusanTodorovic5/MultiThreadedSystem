#include "pcblist.h"
#include "pcb.h"
#include "system.h"
PCBList::PCBList(){
	this->head = 0;
	this->tail = 0;
}
void PCBList::insert(PCB* pcb){
	if (!head){
		head = new PCBNode(pcb);
		tail = head;
		return;
	}
	else{
		PCBNode* res = new PCBNode(pcb);
		tail->next = res;
		tail = tail->next;
	}
}
void PCBList::remove(int id){
	if (head->data->myId == id){
		if (head->next == 0){
			head = 0;
			tail = 0;
			return;
		}
		head = head->next;
		if (head->next == 0){
			tail = head;
		}
	}
	PCBNode* last = head;
	for (PCBNode* p = head->next;p != 0;p = p->next){
		if (id == p->data->myId){
			if (p->next != 0){
				last->next = p->next;
				delete p;
				return;
			}
			else{
				last-> next = 0;
				tail = last;
				delete p;
				return;
			}
		}
		last = p;
	}
}
PCB* PCBList::get(int id){
	for (PCBNode* p = head;p != 0;p = p->next){
		if (p->data->myId == id){
			return p->data;
		}
	}
	return 0;
}
PCB* PCBList::getFirst(){
	if (head){
		PCB* t = head->data;
		head = head->next;
		return t;
	}
	return 0;
}
void PCBList::waitAll(){
	for (PCBNode* p = head;p != 0;p = p->next){
		if (p->data->myThread != 0){
			p->data->myThread->waitToComplete();
		}
		else{
			System::lockFlag = 0;
			if (p->data->state == FINISHED || System::running->state == FINISHED){
				System::lockFlag = 1;
				return;
			}
			System::running->state = BLOCKED;
			p->data->waitList->insert(System::running);
			System::lockFlag = 1;
			dispatch();
		}
	}
}
PCBList::~PCBList(){
	for (PCBNode* p = head;p != 0;){

		PCBNode* res = p;
		p = p->next;

		delete res;
	}
}
