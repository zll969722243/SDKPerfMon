#include "IPCDingTalkHelper.h"
#include "MyServiceInvokerObserver.h"
#include "MyRequestContext.h"
#include "alilang_vpn_config.h"

IPCDingTalkHelper::IPCDingTalkHelper()
{
	m_hIPCCoreMod = NULL;

	m_pfunInitIPCCoreRuntime = NULL;
	m_pfunCreateServiceHost = NULL;
	m_pfunCreateServiceInvoker = NULL;
	m_pfunDestroyServiceHost = NULL;
	m_pfunDestroyServiceInvoker = NULL;

	m_pServiceInvoker = NULL;
}

IPCDingTalkHelper::~IPCDingTalkHelper()
{

}

bool IPCDingTalkHelper::Init()
{
	m_hIPCCoreMod = LoadLibraryA("IPCDingTalk.dll");
	if (!m_hIPCCoreMod)
	{
		DWORD dwErrorCode = GetLastError();
		return false;
	}

	m_pfunInitIPCCoreRuntime = (pfunInitIPCCoreRuntime)GetProcAddress(m_hIPCCoreMod, "InitIPCCoreRuntime");
	m_pfunCreateServiceHost = (pfunCreateServiceHost)GetProcAddress(m_hIPCCoreMod, "CreateServiceHost");
	m_pfunCreateServiceInvoker = (pfunCreateServiceInvoker)GetProcAddress(m_hIPCCoreMod, "CreateServiceInvoker");
	m_pfunDestroyServiceHost = (pfunDestroyServiceHost)GetProcAddress(m_hIPCCoreMod, "DestroyServiceHost");
	m_pfunDestroyServiceInvoker = (pfunDestroyServiceInvoker)GetProcAddress(m_hIPCCoreMod, "DestroyServiceInvoker");

	if (!m_pfunInitIPCCoreRuntime || !m_pfunCreateServiceHost || !m_pfunCreateServiceInvoker || \
		!m_pfunDestroyServiceHost || !m_pfunDestroyServiceInvoker)
	{
		return false;
	}

	bool bOk = m_pfunInitIPCCoreRuntime(nullptr);
	if (!bOk)
	{
		return false;
	}

	IServiceInvokerObserver* observer(new MyServiceInvokerObserver);
	bOk = m_pfunCreateServiceInvoker(SERVICE_HOST_NAME_ALILANG, APP_NAME_ALILANG, HANDSHAKE_SESSION_ID_ALILANG, SERVER_PORT_START_ALILANG, observer, &m_pServiceInvoker);
	if (!bOk || !m_pServiceInvoker)
	{
		return false;
	}

	return true;
}

bool IPCDingTalkHelper::UnInit()
{
	if (m_pServiceInvoker)
		m_pServiceInvoker->Stop();

	if(m_pfunDestroyServiceInvoker)
		m_pfunDestroyServiceInvoker(m_pServiceInvoker);

	if (m_hIPCCoreMod)
		FreeLibrary(m_hIPCCoreMod);

	m_hIPCCoreMod = NULL;

	m_pfunInitIPCCoreRuntime = NULL;
	m_pfunCreateServiceHost = NULL;
	m_pfunCreateServiceInvoker = NULL;
	m_pfunDestroyServiceHost = NULL;
	m_pfunDestroyServiceInvoker = NULL;

	return true;
}

bool IPCDingTalkHelper::InvokeServer(const char* uri, const char* body)
{
	if (!uri || !body)
		return false;

	if (!m_pServiceInvoker)
		return false;

	IRequestContext* pMyRequestContext(new MyRequestContext);
	pMyRequestContext->SetTimeout(10000);//ms
	m_pServiceInvoker->InvokeService(uri, body, pMyRequestContext);

	return true;
}

