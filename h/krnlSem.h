//Kernel semaphore
#ifndef KERNEL_SEMAPHORE
#define KERNEL_SEMAPHORE
class SemList;
class PCB;
class WaitList;
class KernelSem{
public:
	static int _id;
	int id;
	KernelSem(int init = 1);
	~KernelSem();
	int wait(unsigned int maxTimeToWait);
	void signal();
	void decrement();
	int val() const;
private:
	WaitList* blockedList;
	int value;
};

#endif
