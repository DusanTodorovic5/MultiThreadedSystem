#ifndef IVTENTRY_CLASS
#define IVTENTRY_CLASS
typedef void interrupt (*pInterrupt)(...);
class KernelEv;
class IVTEntry{
public:
	IVTEntry(unsigned char, pInterrupt);
	~IVTEntry();

	void signal();
	void oldRoutine();

	KernelEv* event;
	pInterrupt old;
	unsigned char ivtNo;
};

#endif
