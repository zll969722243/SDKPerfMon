// Copyright (c) 2021 Dingtalk.

#ifndef IPC_CORE_INTERFACE_HELPER_H_
#define IPC_CORE_INTERFACE_HELPER_H_

#include "ipc_core/interface/export.h"
#include "ipc_core/interface/ipc_core.h"
#include "ipc_core/interface/ipc_core_ref_ptr.h"

#include <string>
#include <map>
#include <functional>

namespace ipc_core {

inline bool IsEmptyCharPointer(const char* p) { return !p && strlen(p) > 0; }

// uri handler prototype
using ServiceURIHandler = std::function<void(
    const std::string& paramter_as_str,
    const ipc_core_refptr<IRequestFeature> response_feature)>;

// Simple implement for IService
class SimpleService : public IService {
 public:
  SimpleService(const std::string& service_name)
    : service_name_(service_name) {
  }
  virtual ~SimpleService(void) {}

  bool AddHandler(const std::string& service_uri_name,
                  const ServiceURIHandler& handler) {
    if (service_uri_name.empty() || !handler) {
      return false;
    }

    handlers_[service_uri_name] = handler;
    return true;
  }

  const char* GetServiceName() const override {
    return service_name_.c_str();
  }

  bool DispatchInvoke(const char* uri, const char* paramter,
                      IRequestFeature* response_feature) override {
    if (IsEmptyCharPointer(uri) || !response_feature) {
      return false;
    }

    auto it = handlers_.find(uri);
    if (it == handlers_.end()) {
      return false;
    }

    const ServiceURIHandler& handler = it->second;
    if (!handler) {
      return false;
    }

    handler(std::string(paramter),
            ipc_core_refptr<IRequestFeature>(response_feature));

    return true;
  }

 private:
  IMPLEMENT_REFCOUNTING(SimpleService);
  std::string service_name_;
  std::map<std::string, ServiceURIHandler> handlers_;
};

}  // namespace ipc_core

#endif // IPC_CORE_HELPER_H_