#ifndef _MY_REQUEST_CONTEXT_H_
#define _MY_REQUEST_CONTEXT_H_

#include "ipc_core/interface/ipc_core.h"
#include "ipc_core/interface/ipc_core_ref_ptr.h"

using namespace ipc_core;

class MyRequestContext : public IRequestContext
{
private:
	IMPLEMENT_REFCOUNTING(MyRequestContext);

public:
	MyRequestContext();
	virtual ~MyRequestContext();

public:
	void OnSuccess(const char* body) override;
	void OnFailure(const int32_t ErrorCode, const char* ErrorMsg) override;
	void OnTimedout() override;
};

#endif //_MY_REQUEST_CONTEXT_H_
