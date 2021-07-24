// Copyright (c) 2021 Dingtalk.

#ifndef IPC_CORE_INTERFACE_IPC_CORE_REF_PTR_H_
#define IPC_CORE_INTERFACE_IPC_CORE_REF_PTR_H_

#include <atomic>
#include <cassert>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)
#endif  // DISALLOW_COPY_AND_ASSIGN

class CoreRefCount {
 public:
  CoreRefCount() : ref_count_(0) {}

  void AddRef() const { ++ref_count_; }

  bool Release() const { return --ref_count_ == 0; }

 private:
  mutable std::atomic<int32_t> ref_count_;
  DISALLOW_COPY_AND_ASSIGN(CoreRefCount);
};

#define IMPLEMENT_REFCOUNTING(ClassName)                \
 public:                                                \
  void AddRef() const override { ref_count_.AddRef(); } \
  void Release() const override {                       \
    if (ref_count_.Release()) {                         \
      delete static_cast<const ClassName*>(this);       \
      return;                                           \
    }                                                   \
    return;                                             \
  }                                                     \
                                                        \
 private:                                               \
  CoreRefCount ref_count_


// Helper for refptr
template <class T>
class ipc_core_refptr {
 public:
  typedef T element_type;

  ipc_core_refptr() : ptr_(NULL) {}

  ipc_core_refptr(T* p) : ptr_(p) {
    if (ptr_) AddRef(ptr_);
  }

  // Copy constructor.
  ipc_core_refptr(const ipc_core_refptr<T>& r) : ptr_(r.ptr_) {
    if (ptr_) AddRef(ptr_);
  }

  // Copy conversion constructor.
  template <typename U>
  ipc_core_refptr(const ipc_core_refptr<U>& r) : ptr_(r.get()) {
    if (ptr_) AddRef(ptr_);
  }

  // Move constructor. This is required in addition to the conversion
  // constructor below in order for clang to warn about pessimizing moves.
  ipc_core_refptr(ipc_core_refptr&& r) : ptr_(r.get()) { r.ptr_ = nullptr; }

  // Move conversion constructor.
  template <typename U>
  ipc_core_refptr(ipc_core_refptr<U>&& r) : ptr_(r.get()) {
    r.ptr_ = nullptr;
  }

  ~ipc_core_refptr() {
    if (ptr_) Release(ptr_);
  }

  T* get() const { return ptr_; }

  T& operator*() const {
    assert(ptr_ != NULL);
    return *ptr_;
  }

  T* operator->() const {
    assert(ptr_ != NULL);
    return ptr_;
  }

  ipc_core_refptr<T>& operator=(T* p) {
    // AddRef first so that self assignment should work
    if (p) AddRef(p);
    T* old_ptr = ptr_;
    ptr_ = p;
    if (old_ptr) Release(old_ptr);
    return *this;
  }

  ipc_core_refptr<T>& operator=(const ipc_core_refptr<T>& r) {
    return *this = r.ptr_;
  }

  template <typename U>
  ipc_core_refptr<T>& operator=(const ipc_core_refptr<U>& r) {
    return *this = r.get();
  }

  ipc_core_refptr<T>& operator=(ipc_core_refptr<T>&& r) {
    ipc_core_refptr<T>(std::move(r)).swap(*this);
    return *this;
  }

  template <typename U>
  ipc_core_refptr<T>& operator=(ipc_core_refptr<U>&& r) {
    ipc_core_refptr<T>(std::move(r)).swap(*this);
    return *this;
  }

  void swap(T** pp) {
    T* p = ptr_;
    ptr_ = *pp;
    *pp = p;
  }

  void swap(ipc_core_refptr<T>& r) { swap(&r.ptr_); }

 private:
  template <typename U>
  friend class ipc_core_refptr;

  // Allow core_refptr<T> to be used in boolean expressions, but not
  // implicitly convertible to a real bool (which is dangerous).
  //
  // Note that this trick is only safe when the == and != operators
  // are declared explicitly, as otherwise "refptr1 == refptr2"
  // will compile but do the wrong thing (i.e., convert to Testable
  // and then do the comparison).
  typedef T* ipc_core_refptr::*Testable;

 public:
  operator Testable() const { return ptr_ ? &ipc_core_refptr::ptr_ : nullptr; }

  template <typename U>
  bool operator==(const ipc_core_refptr<U>& rhs) const {
    return ptr_ == rhs.get();
  }

  template <typename U>
  bool operator!=(const ipc_core_refptr<U>& rhs) const {
    return !operator==(rhs);
  }

  template <typename U>
  bool operator<(const ipc_core_refptr<U>& rhs) const {
    return ptr_ < rhs.get();
  }

 protected:
  T* ptr_;

 private:
  // Non-inline helpers to allow:
  //     class Opaque;
  //     extern template class core_refptr<Opaque>;
  // Otherwise the compiler will complain that Opaque is an incomplete type.
  static void AddRef(T* ptr);
  static void Release(T* ptr);
};

template <typename T>
void ipc_core_refptr<T>::AddRef(T* ptr) {
  ptr->AddRef();
}

template <typename T>
void ipc_core_refptr<T>::Release(T* ptr) {
  ptr->Release();
}

// Handy utility for creating a core_refptr<T> out of a T* explicitly without
// having to retype all the template arguments
template <typename T>
ipc_core_refptr<T> make_core_refptr(T* t) {
  return ipc_core_refptr<T>(t);
}

// Temporary operator overloads to facilitate the transition. See
// https://crbug.com/110610.
template <typename T, typename U>
bool operator==(const ipc_core_refptr<T>& lhs, const U* rhs) {
  return lhs.get() == rhs;
}

template <typename T, typename U>
bool operator==(const T* lhs, const ipc_core_refptr<U>& rhs) {
  return lhs == rhs.get();
}

template <typename T, typename U>
bool operator!=(const ipc_core_refptr<T>& lhs, const U* rhs) {
  return !operator==(lhs, rhs);
}

template <typename T, typename U>
bool operator!=(const T* lhs, const ipc_core_refptr<U>& rhs) {
  return !operator==(lhs, rhs);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const ipc_core_refptr<T>& p) {
  return out << p.get();
}

#endif // IPC_CORE_INTERFACE_IPC_CORE_REF_PTR_H_