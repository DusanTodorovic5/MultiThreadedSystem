#ifndef PCB_LIST_CLASS
#define PCB_LIST_CLASS
class PCB;
class PCBList{
public:
	PCBList();
	PCB* get(int id);
	PCB* getFirst();
	void remove(int id);
	void insert(PCB*);
	void waitAll();
	~PCBList();
private:
	struct PCBNode{
		PCB* data;
		PCBNode* next;
		PCBNode(PCB* p){
			data = p;
			next = 0;
		}
	} *head,*tail;
};

#endif
