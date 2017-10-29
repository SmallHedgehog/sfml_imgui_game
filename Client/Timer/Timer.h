#pragma once

#include <iostream>
// #include <time.h>
#include <ctime>
#include <functional>
#include <thread>


class Timer
{
public:
	Timer(std::function< void(bool)> _isComplete, std::function< void(int)> _callbackFunc):isTimerStart(false)
										 { isComplete = _isComplete; callbackFunc = _callbackFunc; _start_time = std::clock(); }
	~Timer()							 { Join(); }
	inline double elapsed_min() const	 { return double(1) / double(CLOCKS_PER_SEC); }
	inline double elapsed_max() const	 { return (double((std::numeric_limits<std::clock_t>::max)()) - double(_start_time)) / double(CLOCKS_PER_SEC); }
	/* inline void start()				 { _start_time = std::clock(); } */
	inline void restart()				 { _start_time = std::clock(); }
	inline double elapsed() const		 { return double(std::clock() - _start_time) / CLOCKS_PER_SEC; }
	inline std::clock_t elapsedl() const { return (std::clock() - _start_time); }
	inline bool GetIsTimerEnd() const	 { return !isTimerStart; }

	/* Positive timer */
	inline void PosTimer(int _start, int _end /*, std::function< void(void)> isComplete, std::function< void(int)> callbackFunc */)
	{
		int start = _start;
		int end	 = _end;

		if (_start >= _end)
			throw "params1 must more than params2";

		isTimerStart = true;
		while (true)
		{
			callbackFunc(start);
			if (start >= end)
			{
				isComplete(true);
				isTimerStart = false;
				break;
			}
			else
			{
				if (elapsedl() >= (std::clock_t)CLOCKS_PER_SEC)
				{
					callbackFunc(++start);
					restart();
				}
			}
		}
	}
	/* Negative timer */
	inline void NegTimer(int _start, int _end /*, std::function< void(void)> isComplete, std::function< void(int)> callbackFunc */)
	{
		int start = _start;
		int end	 = _end;

		if (_start < _end)
			throw "params1 must less than params2";

		isTimerStart = true;
		while (true)
		{
			callbackFunc(start);
			if (start <= end)
			{
				isComplete(true);
				isTimerStart = false;
				break;
			}
			else
			{
				if (elapsedl() >= (std::clock_t)CLOCKS_PER_SEC)
				{
					callbackFunc(--start);
					restart();
				}
			}
		}
	}

	/* Begin with a new thread of NegTimer */
	inline void BEGIN_NegTimer(int start, int end)
	{
		restart();
		_t = std::thread(&Timer::NegTimer, this, start, end);
		_t.detach();
	}

	/* Begin with a new thread of PosTimer */
	inline void BEGIN_PosTimer(int start, int end)
	{
		restart();
		_t = std::thread(&Timer::PosTimer, this, start, end);
	}

	inline void Join()
	{
		if (_t.joinable())
			_t.join();
	}

	/* Judge whether pass a second */
	inline bool IsPassASecond() const { return ((std::clock() - _start_time) == (std::clock_t)CLOCKS_PER_SEC) ? true : false; }

private:
	bool isTimerStart;
	std::clock_t _start_time;
	std::function< void(bool)>	isComplete;
	std::function< void(int)>	callbackFunc;

	std::thread _t;
};
