#include "system.h"
//#include "user.h"
extern void inic();
extern void restore();
extern int userMain(int argc, char* argv[]);
class UserThread : public Thread{
public:
	UserThread(int argc, char** argv):Thread(4096,2){
		this->argc = argc;
		this->argv = argv;
	}
	void run(){
		int t = userMain(argc,argv);
	}
	Thread* clone() const{
		return new UserThread(argc,argv);
	}
	~UserThread(){ }
private:
	int argc;
	char** argv;
};
int main(int argc, char* argv[]){
	inic();

	System::allPCB = new PCBList();

	System::mainPCB = new PCB(0,4096,10);
	System::mainPCB->state = READY;

	System::running = System::mainPCB;

	System::idle = new IdleThread();
	System::idle->start();

	System::allSem = new SemList();

	UserThread* ut = new UserThread(argc,argv);
	ut->start();
	ut->waitToComplete();
	delete ut;
	System::running = System::mainPCB;

	restore();

	delete System::allPCB;
	delete System::mainPCB;
	delete System::allSem;

	return 0;
}
