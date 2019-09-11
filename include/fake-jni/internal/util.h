#pragma once

#include <mutex>
#include <map>

namespace FakeJni {
 template<typename T>
 class List {
 public:
  T entry;
  List<T> *next;
 };

 //Default deallocator generator
 namespace _CX {
  template<typename T>
  class Deallocator final {
  public:
   [[gnu::always_inline]]
   inline static void deallocate(void * const ptr) {
    delete ((T *)ptr);
   }
  };
 }

 //TODO should AllocStack support custom allocators (for List<T>)?
 //TODO add range-based-for support, both blocking and non-blocking.
 // - Block on all removal invocations while something is iterating over the stack
 template<typename T>
 class AllocStack final {
 private:
  class Entry {
  public:
   using deallocator_t = void (* const)(void *);
   deallocator_t dealloc;
   T t;

   Entry(deallocator_t dealloc, T t):
    dealloc(dealloc),
    t(t)
   {}
  };

  List<Entry>
   *head = nullptr,
   *tail = nullptr,
   **next = nullptr;
  uint32_t size = 0;
  std::mutex m;

 public:
  const bool dealloc;

  AllocStack(bool dealloc = false) noexcept : dealloc(dealloc) {}

  ~AllocStack() {
   List<Entry> *elem = head;
   while (elem != nullptr) {
    if (dealloc) {
     const auto deallocator = elem->entry.dealloc;
     if (deallocator) {
      deallocator(elem->entry.t);
     } else {
      delete elem->entry.t;
     }
    }
    List<Entry> *copy = elem;
    elem = copy->next;
    delete copy;
   }
  }

  T pushAlloc(T t) {
   return pushAlloc(nullptr, t);
  }

  //Pushes a new allocation to the end of the list, with a custom deallocator
  T pushAlloc(void (* const deallocator)(void *), T t) {
   std::scoped_lock<std::mutex> mLock(m);
   size += 1;
   auto list = new List<Entry>{{deallocator, t}, nullptr};
   if (!head) {
    head = list;
   } else {
    *next = list;
   }
   tail = list;
   next = &(list->next);
   return t;
  }

  T popAlloc() {
   std::scoped_lock<std::mutex> mLock(m);
   if (!head) {
    throw std::out_of_range("Allocation table is empty!");
   }
   List<Entry> *elem = head;
   while (elem && elem != tail) {
    elem = elem->next;
   }
   size -= 1;
   T const t = tail->entry.t;
   tail = elem;
   next = &(tail->next);
   delete tail;
   return t;
  }

  bool removeAlloc(T t) {
   std::scoped_lock<std::mutex> mLock(m);
   List<Entry> *elem = head, *last = nullptr;
   while (elem && elem->entry.t != t) {
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
    const auto deallocator = elem->entry.dealloc;
    if (deallocator) {
     deallocator(elem->entry.t);
    } else {
     delete elem->entry.t;
    }
   }
   delete elem;
   return true;
  }

// bool removeAlloc(uint32_t index) {
//  return removeAlloc((*this)[index]);
// }

  void clear() {
   for (uint32_t i = size; i > 0; i--) {
    removeAlloc((*this)[0]);
   }
  }

  T& operator[](uint32_t index) {
   std::scoped_lock<std::mutex> mLock(m);
   List<Entry> *elem = head;
   while (elem && index > 0) {
    elem = elem->next;
    index -= 1;
   }
   if (index != 0) {
    throw std::out_of_range("Index out of range!");
   }
   return elem->entry.t;
  }

  const T& operator[](uint32_t index) const {
   auto non_const_ptr = const_cast<AllocStack<T> *>(this);
   return (*non_const_ptr)[index];
  }

  uint32_t getSize() const noexcept {
   return size;
  }
 };
}