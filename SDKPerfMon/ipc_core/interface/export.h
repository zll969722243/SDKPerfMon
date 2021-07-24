// Copyright (c) 2021 Dingtalk.

#ifndef IPC_CORE_INTERFACE_EXPORT_H_
#define IPC_CORE_INTERFACE_EXPORT_H_

#if defined(WIN32)
#if defined(IPCCORE_EXPORTS)
#define IPC_CORE_EXPORT __declspec(dllexport)
#else
#define IPC_CORE_EXPORT __declspec(dllimport)
#endif  // defined(IPCCORE_EXPORTS)


#else  // defined(WIN32)

#if defined(IPCCORE_EXPORTS)
#define IPC_CORE_EXPORT __attribute__((visibility("default")))
#else
#define IPC_CORE_EXPORT
#endif

#endif


#endif  // IPC_CORE_INTERFACE_EXPORT_H_
