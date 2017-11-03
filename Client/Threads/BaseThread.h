#pragma once

#include <thread>

class BaseThread
{
public:
	void start()
	{
		_t = std::thread(&threadRoutine, this);
	}

	void join()
	{
		if (_t.joinable()) _t.join();
	}

	virtual void run() = 0;

protected:
	std::thread _t;

	static void* threadRoutine(void* args)
	{
		BaseThread* Thread = static_cast<BaseThread*>(args);
		Thread->run();

		return ((void*)0);
	}
};
