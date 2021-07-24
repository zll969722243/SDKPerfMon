#include "MyRequestContext.h"
#include "SDKPerfMon.h"

MyRequestContext::MyRequestContext()
{
}

MyRequestContext::~MyRequestContext()
{
}

void MyRequestContext::OnSuccess(const char* body)
{
    SDKPerfMon::m_nSuccessNums++;

	return;
}

void MyRequestContext::OnFailure(const int32_t ErrorCode, const char* ErrorMsg)
{
    SDKPerfMon::m_nFailureNums++;

	return;
}

void MyRequestContext::OnTimedout()
{
    SDKPerfMon::m_nTimedoutNums++;

	return;
}

