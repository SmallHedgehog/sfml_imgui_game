#include "MsgDealThread.h"


MsgDealThread::MsgDealThread(MsgQueue* _msgQueue, std::function< void(DATA_PACKAGE&)> _callbackFunc):
	msgQueue(_msgQueue)
{
	callbackFunc = _callbackFunc;
}

void MsgDealThread::run()
{
	DATA_PACKAGE dPackage;
	while (true)
	{
		if (msgQueue)
		{
			(*msgQueue) >> dPackage;
			callbackFunc(dPackage);
			if (dPackage.dataHeader.smsgType == SMessageTypeFlags_::SMessageTypeFlags_IsTheUserSingout)
				break;
		}
		else
			break;
	}
}
