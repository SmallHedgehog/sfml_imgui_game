#pragma once

#include <thread>

class virtualThread
{
public:
	void start();
	void join();
	virtual void run() = 0;

protected:
	std::thread _t;

protected:
	static void* threadRoutine(void* args);
};
