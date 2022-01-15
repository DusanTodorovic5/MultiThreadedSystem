#include "waitlist.h"
#include "pcb.h"
WaitList::WaitList(){
	head = 0;
}
WaitList::~WaitList(){
	while (head){
		WaitElem* we = head;
		head = head->next;
		delete we;
	}
}
void WaitList::insert(PCB* p, int t){
	if (head == 0){
		head = new WaitElem(p, t);
	}
	else{
		WaitElem* c = head;
		while (c->next)
			c = c->next;
		WaitElem* n = new WaitElem(p,t);
		c->next = n;
	}
}
PCB* WaitList::remove(int id){
	WaitElem* curr = head;
	WaitElem* last = 0;
	while (curr != 0) {
		if (curr->data->myId == id) {
			if (last != 0) {
				last->next = curr->next;
				PCB* rt = curr->data;
				delete curr;
				return rt;
			} else {
				head = head->next;
				PCB* rt = curr->data;
				delete curr;
				return rt;
			}
		}
		last = curr;
		curr = curr->next;
	}
	return 0;
}
WaitList::WaitElem* WaitList::getHead(){
	return head;
}
PCB* WaitList::removeFirst(){
	if (head != 0){
		WaitElem* curr = head;
		head = head->next;
		PCB* p = curr->data;
		delete curr;
		return p;
	}
	return 0;
}
