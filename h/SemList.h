#ifndef SEM_LIST
#define SEM_LIST
class KernelSem;
class SemList{
public:
	SemList();
	void remove(int);
	void insert(KernelSem*);
	void decAll();
	~SemList();
private:
	struct SemElem{
		KernelSem* data;
		SemElem* next;
		SemElem(KernelSem* ks){
			data = ks;
			next = 0;
		}
	};
	SemElem* head;
};

#endif
