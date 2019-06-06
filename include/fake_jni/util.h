#pragma once

#include <mutex>

namespace FakeJni {
 template<typename T>
 class List {
 public:
  T t;
  List<T> *next;
 };

 template<typename T>
 class AllocStack final {
 private:
  List<T> *head = nullptr, *tail = nullptr, **next = nullptr;
  uint32_t size = 0;
  std::mutex m;

 public:
  const bool dealloc;
  //TODO
//  template<typename... Args>
//  explicit AllocStack(Args... args): AllocStack() {
//   static_assert((std::is_same<T, Args>::value && ...));
//   //Requires recursive template decomposition (CX)
//   (pushAlloc(args)...);
//  };

  AllocStack(): AllocStack(false) {}

  AllocStack(const bool dealloc): dealloc(dealloc) {}

  ~AllocStack() {
   List<T> *elem = head;
   while(elem != nullptr) {
    if (dealloc) {
     delete elem->t;
    }
    List<T> *copy = elem;
    elem = copy->next;
    delete copy;
   }
  }

  //Pushes a new allocation to the end of the list
  T pushAlloc(T t) {
   std::scoped_lock<std::mutex> mLock(m);
   size += 1;
   auto * const list = new List<T> { t, nullptr };
   if (!head) {
    head = list;
   } else {
    *next = list;
    next = &(list->next);
   }
   return t;
  }

  T popAlloc() {
   std::scoped_lock<std::mutex> mLock(m);
   if (!head) {
    throw std::out_of_range("Allocation table is empty!");
   }
   List<T> *elem = head;
   while (elem && elem != tail) {
    elem = elem->next;
   }
   size -= 1;
   T const t = tail->t;
   tail = elem;
   next = &(tail->next);
   delete tail;
   return t;
  }

  bool removeAlloc(T t) {
   std::scoped_lock<std::mutex> mLock(m);
   List<T> *elem = head, *last = nullptr;
   while (elem && elem->t != t) {
    last = elem;
    elem = elem->next;
   }
   if (!elem) {
    return false;
   }
   size -= 1;
   if (elem->next && last) {
    last->next = elem->next;
   } else if (size == 0) {
    head = nullptr;
   }
   if (dealloc) {
    delete elem->t;
   }
   delete elem;
   return true;
  }

  bool removeAlloc(uint32_t index) {
   return removeAlloc((*this)[index]);
  }

  T operator[](uint32_t index) {
   std::scoped_lock<std::mutex> mLock(m);
   List<T> *elem = head;
   while (elem && index > 0) {
    elem = elem->next;
    index -= 1;
   }
   if (!elem) {
    //TODO message
    throw std::out_of_range("");
   }
   return elem->t;
  }

  uint32_t getSize() {
   return size;
  }
 };

 //JNI Signature generation utils
 namespace _CX {

 }
}