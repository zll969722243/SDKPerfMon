#ifndef _MY_SERVICE_INVOKER_OBSERVER_H_
#define _MY_SERVICE_INVOKER_OBSERVER_H_

#include "ipc_core/interface/ipc_core.h"
#include "ipc_core/interface/ipc_core_ref_ptr.h"

using namespace ipc_core;

class MyServiceInvokerObserver : public IServiceInvokerObserver
{
private:
	IMPLEMENT_REFCOUNTING(MyServiceInvokerObserver);

public:
	MyServiceInvokerObserver();
	virtual ~MyServiceInvokerObserver();

public:
	void OnServiceInvokerInit(IServiceInvoker* pServiceInvoker) override;
	void OnServiceInvokerStartSuccess(IServiceInvoker* pServiceInvoker) override;
	void OnServiceInvokerStartFailure(IServiceInvoker* pServiceInvoker, const int32_t ErrorCode) override;
	void OnServiceInvokerStoped(IServiceInvoker* pServiceInvoker) override;
};

#endif //_MY_SERVICE_INVOKER_OBSERVER_H_
