#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "IPCDingTalkHelper.h"
#include <QThread>

class Monitor:public QThread
{
public:
	Monitor();
	~Monitor();

public:
	bool Init();
	bool UnInit();

//signals:

protected:
	virtual void run();

public:
    void setMaxReqs(int nMaxNum);

private:
    int m_nMaxReqs;
    IPCDingTalkHelper m_IPCDingTalkHelper;
};

#endif //_MONITOR_H_