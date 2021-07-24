#include "MyServiceInvokerObserver.h"


MyServiceInvokerObserver::MyServiceInvokerObserver()
{
}

MyServiceInvokerObserver::~MyServiceInvokerObserver()
{
}

void MyServiceInvokerObserver::OnServiceInvokerInit(IServiceInvoker* pServiceInvoker)
{
	return;
}

void MyServiceInvokerObserver::OnServiceInvokerStartSuccess(IServiceInvoker* pServiceInvoker)
{

	return;
}

void MyServiceInvokerObserver::OnServiceInvokerStartFailure(IServiceInvoker* pServiceInvoker, const int32_t ErrorCode)
{

	return;
}

void MyServiceInvokerObserver::OnServiceInvokerStoped(IServiceInvoker* pServiceInvoker)
{

	return;
}

