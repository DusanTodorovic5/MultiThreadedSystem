#ifndef WAIT_LIST_H
#define WAIT_LIST_H
class PCB;
class WaitList {

public:
	WaitList();
	~WaitList();
	struct WaitElem {
		PCB* data;
		int time;
		WaitElem* next;
		WaitElem(PCB* p, int t) {
			data = p;
			time = t;
			next = 0;
		}
	};
	void insert(PCB*, int);
	PCB* remove(int);
	PCB* removeFirst();
	WaitElem* getHead();
private:
	WaitElem* head;

};

#endif
