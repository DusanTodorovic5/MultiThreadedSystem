#include "SemList.h"
#include "krnlSem.h"
SemList::SemList(){
	head = 0;
}
void SemList::remove(int id){
	SemElem* curr = head;
	SemElem* last = 0;
	while (curr != 0){
		if (curr->data->id == id){
			if (last != 0){
				last->next = curr->next;
				delete curr;
				return;
			}
			else{
				head = head->next;
				delete curr;
				return;
			}
		}
		last = curr;
		curr = curr->next;
	}
}
void SemList::insert(KernelSem* sem){
	if (this->head == 0){
		SemElem* sm = new SemElem(sem);
		this->head = sm;
	}
	else{
		SemElem* curr = this->head;
		while (curr->next != 0)
			curr = curr->next;
		SemElem* rez = new SemElem(sem);
		curr->next = rez;
	}
}
void SemList::decAll(){
	SemElem* curr = head;
	while (curr != 0){
		curr->data->decrement();
		curr = curr->next;
	}
}
SemList::~SemList(){
	while (head != 0){
		SemElem* rez = head;
		head = head->next;
		delete rez;
	}
}
