#include "MyRequestContext.h"
#include "SDKPerfMon.h"

#include <QMutex>

static QMutex mutex_suc;
static QMutex mutex_fail;
static QMutex mutex_out;

MyRequestContext::MyRequestContext()
{
}

MyRequestContext::~MyRequestContext()
{
}

void MyRequestContext::OnSuccess(const char* body)
{
    mutex_suc.lock();
    SDKPerfMon::m_nSuccessNums++;
    mutex_suc.unlock();

	return;
}

void MyRequestContext::OnFailure(const int32_t ErrorCode, const char* ErrorMsg)
{
    mutex_fail.lock();
    SDKPerfMon::m_nFailureNums++;
    mutex_fail.unlock();

	return;
}

void MyRequestContext::OnTimedout()
{
    mutex_out.lock();
    SDKPerfMon::m_nTimedoutNums++;
    mutex_out.unlock();

	return;
}

