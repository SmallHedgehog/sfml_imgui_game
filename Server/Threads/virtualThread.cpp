#include "virtualThread.h"


void virtualThread::start()
{
	_t = std::thread(&threadRoutine, this);
}

void virtualThread::join()
{
	if (_t.joinable())
		_t.join();
}

void* virtualThread::threadRoutine(void* args)
{
	virtualThread* Thread = static_cast<virtualThread*>(args);
	Thread->run();

	return ((void*)0);
}
