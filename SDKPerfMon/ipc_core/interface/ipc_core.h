// Copyright (c) 2021 Dingtalk.

#ifndef IPC_CORE_INTERFACE_IPC_CORE_H_
#define IPC_CORE_INTERFACE_IPC_CORE_H_

#include <cstdint>
#include <memory>
#include <atomic>
#include <string>

#include "ipc_core/interface/export.h"

namespace ipc_core {

class IRefCount {
 public:
  virtual void AddRef() const = 0;
  virtual void Release() const = 0;

 protected:
  virtual ~IRefCount() {}
};

///////////////////////////////////////////////////////////////////////////////

class IRequestFeature : public IRefCount {
 public:
  IRequestFeature() {}
  virtual ~IRequestFeature(void) {}

  virtual void DoSuccess(const char* data) = 0;
  virtual void DoFailure(int32_t error_code, const char* error_msg) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class IService : public IRefCount {
 public:
  IService() {}
  virtual ~IService(void) {}

  virtual const char* GetServiceName() const = 0;
  virtual bool DispatchInvoke(const char* uri, const char* paramter,
                              IRequestFeature* response_feature) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class IServiceHost : public IRefCount {
 public:
  IServiceHost() { }
  virtual ~IServiceHost() {}
  virtual const char* GetServiceHostName() const = 0;
  virtual void AddService(IService* service) = 0;
  virtual bool SetOption(const char* key, const void* value) = 0;
  virtual bool GetOption(const char* key, void** value) = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class IServiceHostObserver : public IRefCount {
 public:
  IServiceHostObserver() {}
  virtual ~IServiceHostObserver() {}
  virtual void OnServiceHostInit(IServiceHost* service_host) = 0;
  virtual void OnServiceHostStartSuccess(IServiceHost* service_host,
                                         const int32_t port_number) = 0;
  virtual void OnServiceHostStartFailure(IServiceHost* service_host,
                                         const int32_t error_code) = 0;
  virtual void OnServiceHostStoped(IServiceHost* service_host) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class IRequestContext : public IRefCount {
 public:
  IRequestContext() {}
  virtual ~IRequestContext() {}

  void SetTimeout(int32_t timeout) { timeout_ = timeout; }
  int32_t timeout() const { return timeout_; }

  virtual void OnSuccess(const char* body) {}
  virtual void OnFailure(const int32_t error_code, const char* error_msg) {}
  virtual void OnTimedout() {}

 private:
  int32_t timeout_ = 5000;
};

///////////////////////////////////////////////////////////////////////////////

class IServiceInvoker : public IRefCount {
 public:
  IServiceInvoker() {}
  virtual ~IServiceInvoker() {}
  virtual const char* GetServiceInvokerName() const = 0;
  virtual void InvokeService(const char* uri, const char* body,
                             IRequestContext* request_context) = 0;
  virtual bool SetOption(const char* key, const void* value) = 0;
  virtual bool GetOption(const char* key, void** value) = 0;
  virtual void Stop() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class IServiceInvokerObserver : public IRefCount {
 public:
  IServiceInvokerObserver() {}
  virtual ~IServiceInvokerObserver() {}
  virtual void OnServiceInvokerInit(IServiceInvoker* service_invoker) = 0;
  virtual void OnServiceInvokerStartSuccess(IServiceInvoker* service_invoker) = 0;
  virtual void OnServiceInvokerStartFailure(IServiceInvoker* service_invoker,
                                            const int32_t error_code) = 0;
  virtual void OnServiceInvokerStoped(IServiceInvoker* service_invoker) = 0;
};

///////////////////////////////////////////////////////////////////////////////
typedef void(__cdecl* WriteLogMessageFunc)(int32_t log_level, const char* file,
                                           int32_t line, size_t message_start,
                                           const char* message);

extern "C" IPC_CORE_EXPORT bool __cdecl InitIPCCoreRuntime(WriteLogMessageFunc log);

extern "C" IPC_CORE_EXPORT bool __cdecl CreateServiceHost(
    const char* service_host_name,
    const char* app_name,
    const char* handshake_session_id,
    const int32_t server_port_start,
    IServiceHostObserver* service_host_observer,
    IServiceHost** service_host);

extern "C" IPC_CORE_EXPORT bool __cdecl CreateServiceInvoker(
    const char* service_invoker_name,
    const char* app_name,
    const char* handshake_session_id,
    const int32_t server_port_start,
    IServiceInvokerObserver* service_invoker_observer,
    IServiceInvoker** service_invoker);

extern "C" IPC_CORE_EXPORT void __cdecl DestroyServiceHost(
    IServiceHost* server_host);
extern "C" IPC_CORE_EXPORT void __cdecl DestroyServiceInvoker(
    IServiceInvoker* service_invoker);

} // namespace ipc_core

#endif // IPC_CORE_INTERFACE_IPC_CORE_H_
