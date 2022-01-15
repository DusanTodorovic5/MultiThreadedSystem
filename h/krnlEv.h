#ifndef KERNEL_EVENT_CLASS
#define KERNEL_EVENT_CLASS
class PCB;
class KernelEv{
public:
	KernelEv(unsigned char);
	~KernelEv();
	void signal();
	void wait();
private:
	unsigned char ivtNo;
	int value;
	PCB* kreator;
};

#endif

