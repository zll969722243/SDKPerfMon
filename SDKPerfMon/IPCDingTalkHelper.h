#ifndef _IPC_DINGTALK_HELPER_H_
#define _IPC_DINGTALK_HELPER_H_

#include "ipc_core/interface/ipc_core.h"
#include <Windows.h>

using namespace ipc_core;

class IPCDingTalkHelper
{
public:
	IPCDingTalkHelper();
	~IPCDingTalkHelper();

public:
	bool Init();
	bool UnInit();

public:
	bool InvokeServer(const char* uri, const char* body);

protected:
	typedef void (__cdecl* pfunWriteLogMessageFunc)(int32_t log_level, const char* file,int32_t line, size_t message_start, const char* message);
	typedef bool (__cdecl* pfunInitIPCCoreRuntime)(pfunWriteLogMessageFunc log);
	typedef bool (__cdecl* pfunCreateServiceHost)(const char* service_host_name,const char* app_name,const char* handshake_session_id,const int32_t server_port_start,IServiceHostObserver* service_host_observer,IServiceHost** service_host);
	typedef bool (__cdecl* pfunCreateServiceInvoker)(const char* service_invoker_name,const char* app_name,const char* handshake_session_id,const int32_t server_port_start,IServiceInvokerObserver* service_invoker_observer,IServiceInvoker** service_invoker);
	typedef void (__cdecl* pfunDestroyServiceHost)(IServiceHost* server_host);
	typedef void (__cdecl* pfunDestroyServiceInvoker)(IServiceInvoker* service_invoker);


	pfunInitIPCCoreRuntime m_pfunInitIPCCoreRuntime;
	pfunCreateServiceInvoker m_pfunCreateServiceInvoker;
	pfunDestroyServiceInvoker m_pfunDestroyServiceInvoker;

	pfunCreateServiceHost m_pfunCreateServiceHost;
	pfunDestroyServiceHost m_pfunDestroyServiceHost;

	HMODULE m_hIPCCoreMod;

	IServiceInvoker *m_pServiceInvoker;
};
#endif //_IPC_DINGTALK_HELPER_H_