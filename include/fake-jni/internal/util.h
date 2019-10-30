#pragma once

#include <mutex>
#include <algorithm>
#include <set>
#include <map>
#include <functional>
#include <cstring>

#include <cx/idioms.h>

namespace FakeJni {
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

//Thread safe
// - parallel reads
// - blocking writes (blocks other writes and only blocks reads when the modification is a removal)
 template<typename T>
 class PointerList {
 public:
  constexpr PointerList(bool = false) noexcept {
   static_assert(
    CX::IsPointer<T>::value,
    "PointerList may only encapsulate pointer types!"
   );
  }
 };

 template<typename T>
 class PointerList<T *> {
 private:
  using element_t = T *;

  struct Entry final {
   using deallocator_t = void (*)(void *);
   element_t t;
   const deallocator_t dealloc;
   Entry * next = nullptr;
  };

  //stl iterator support
  //forward access only, maintains iterator validity
  struct Iterator final {
   friend PointerList<element_t>;

   Iterator(const PointerList<element_t> & list, Entry * entry) :
    list(const_cast<PointerList<element_t> &>(list)),
    entry(entry)
   {
    std::scoped_lock iteratorsLock{this->list.iteratorsMutex};
    this->list.iterators.insert(this);
   }

   ~Iterator() {
    std::scoped_lock iteratorsLock{list.iteratorsMutex};
    list.iterators.erase(this);
   }

   Iterator & operator++() {
    if (entry) {
     std::scoped_lock modLock{modMutex};
     entry = entry->next;
    }
    return *this;
   }

   bool operator==(const Iterator & itr) const noexcept {
    return entry == itr.entry;
   }

   bool operator!=(const Iterator & itr) const noexcept {
    return !operator==(itr);
   }

   const element_t & operator*() const {
    if (entry) {
     auto& non_const_ref = const_cast<Iterator &>(*this);
     std::scoped_lock modLock{non_const_ref.modMutex};
     return entry->t;
    }
    throw std::runtime_error("Tried to seek past end of list!");
   }

  private:
   PointerList<element_t> & list;
   Entry * entry;
   std::mutex modMutex;
  };

  friend Iterator;

  inline static const typename Entry::deallocator_t default_deallocator = _CX::Deallocator<T>::deallocate;

  bool dealloc;
  Entry
   *head = nullptr,
   *tail = nullptr;
  std::set<Iterator *> iterators;
  std::mutex
   accessMutex,
   iteratorsMutex;
  uint32_t _size = 0;

  void lockIterators() const {
   auto & non_const_ref = const_cast<PointerList<element_t> &>(*this);
   non_const_ref.iteratorsMutex.lock();
   for (auto & iterator : iterators) {
    iterator->modMutex.lock();
   }
  }

  void unlockIterators() const {
   auto & non_const_ref = const_cast<PointerList<element_t> &>(*this);
   non_const_ref.iteratorsMutex.unlock();
   for (auto & iterator : iterators) {
    iterator->modMutex.unlock();
   }
  }

  void deallocate(Entry * entry) {
   if (dealloc) {
    const auto deallocator = entry->dealloc;
    if (deallocator) {
     deallocator((void *)entry->t);
    }
//    else {
//     delete entry->t;
//    }
   }
  }

 public:
  PointerList(bool dealloc = false) noexcept : dealloc(dealloc) {}

  ~PointerList() {
   Entry *entry = head;
   while (entry) {
    if (dealloc) {
     const auto deallocator = entry->dealloc;
     if (deallocator) {
      deallocator((void *)entry->t);
     }
    }
    Entry *prev = entry;
    entry = entry->next;
    delete prev;
   }
  }

  //stl iterator support
  Iterator begin() const {
   return {*this, head};
  }

  Iterator end() const {
   return {*this, nullptr};
  }

  void insert(element_t t) {
   insert(t, (dealloc ? default_deallocator : nullptr));
  }

  void insert(element_t t, typename Entry::deallocator_t deallocator) {
   std::scoped_lock accessLock{accessMutex};
   _size += 1;
   auto entry = new Entry{t, deallocator, nullptr};
   if (!head) {
    head = entry;
    tail = head;
   } else {
    tail->next = entry;
    tail = entry;
   }
  }

  Iterator erase(element_t t) {
   std::scoped_lock accessLock{accessMutex};
   Entry
    *entry = head,
    *last = nullptr;
   while (entry && entry->t != t) {
    last = entry;
    entry = entry->next;
   }
   if (entry) {
    //lock all active iterators
    lockIterators();
    //replace any iterators pointing to the deleted element with the next element
    for (auto & iterator : iterators) {
     auto & entryPtr = iterator->entry;
     if (entryPtr == entry) {
      entryPtr = entry->next;
     }
    }
    //deallocate element
    _size -= 1;
    deallocate(entry);
    //update entries
    if (_size == 0) {
     head = tail = nullptr;
    } else {
     if (last) {
      last->next = entry->next;
      //if the entry was the tail, set the new tail to the previous entry
      if (!entry->next) {
       tail = last;
      }
     } else {
      //if there was no previous entry, then the entry is head
      //update head to be the next entry
      head = entry->next;
     }
    }
    auto copy = entry;
    //set entry to the next element for the returned iterator
    entry = entry->next;
    delete copy;
    //release all iterator locks
    unlockIterators();
   }
   return {*this, entry};
  }

  void clear() noexcept {
   std::scoped_lock accessLock{accessMutex};
   _size = 0;
   lockIterators();
   //delete all entries and elements
   Entry *entry = head;
   while (entry) {
    deallocate(entry);
    auto copy = entry;
    entry = entry->next;
    delete copy;
   }
   //update entries
   head = tail = nullptr;
   //stop all iterators since the list is now empty
   for (auto & iterator : iterators) {
    iterator->entry = nullptr;
   }
   unlockIterators();
  }

  element_t & operator[](uint32_t index) {
   std::scoped_lock accessLock{accessMutex};
   Entry *entry = head;
   while (entry && index > 0) {
    entry = entry->next;
    index -= 1;
   }
   if (index != 0) {
    throw std::out_of_range("Index out of range!");
   }
   return entry->t;
  }

  const element_t & operator[](uint32_t index) const {
   return const_cast<PointerList<T> &>(*this)[index];
  }

  uint32_t size() const noexcept {
   return _size;
  }

  inline bool contains(const element_t & t) const noexcept {
   for (auto & e : *this) {
    if (e == t) {
     return true;
    }
   }
   return false;
  }

  bool deallocates() const noexcept {
   return dealloc;
  }

  void setDeallocate(bool dealloc) const noexcept {
   const_cast<PointerList<T *> &>(*this).dealloc = dealloc;
  }
 };

#define _CASE_CALLBACK_IF_DEFINED(identifier) \
case #identifier[0]: {\
 const auto& callback = ref.callbacks[#identifier[0]];\
 if (callback) {\
  ref.result.push_back(callback(const_cast<char *>(#identifier)));\
 }\
 break;\
}

#define _JNI_PARSER_TOKEN_CALLBACK \
const auto size = (size_t)(signature - start + 1);\
char *token = new char[size + 1];\
token[size] = '\0';\
strncpy(token, start, size);\
result.push_back(callback(token));\
delete[] token;

 template<typename T>
 struct JniFunctionParser final {
  using callback_t = T (char * token);

  std::function<callback_t>& operator[](char identifier) {
   return callbacks[identifier];
  }

  std::vector<T> parse(const char * signature) const {
   auto& ref = const_cast<JniFunctionParser &>(*this);
   std::scoped_lock lock(ref.parserMutex);
   char parensMatched = -2;
   ref.result.clear();
   while (*signature) {
    switch(*signature) {
     case '(': {
      parensMatched += 1;
      const auto& callback = ref.callbacks['('];
      if (callback) {
       ref.result.push_back(callback(const_cast<char *>("(")));
      }
      break;
     }
     case ')': {
      parensMatched += 1;
      const auto& callback = ref.callbacks[')'];
      if (callback) {
       ref.result.push_back(callback(const_cast<char *>(")")));
      }
      break;
     }
     case 'V': {
      if (parensMatched == -1) {
       throw std::runtime_error("Illegal JNI function prototype, void cannot be function argument!");
      }
      const auto& callback = ref.callbacks['V'];
      if (callback) {
       ref.result.push_back(callback(const_cast<char *>("V")));
      }
      break;
     }
     _CASE_CALLBACK_IF_DEFINED(Z)
     _CASE_CALLBACK_IF_DEFINED(B)
     _CASE_CALLBACK_IF_DEFINED(C)
     _CASE_CALLBACK_IF_DEFINED(S)
     _CASE_CALLBACK_IF_DEFINED(I)
     _CASE_CALLBACK_IF_DEFINED(F)
     _CASE_CALLBACK_IF_DEFINED(J)
     _CASE_CALLBACK_IF_DEFINED(D)
     case 'L': {
      ref.parseClass(signature);
      break;
     }
     case '[': {
      ref.parseArray(signature);
      break;
     }
     default: throw std::runtime_error("Illegal JNI function prototype!");
    }
    signature++;
   }
   if (parensMatched) {
    throw std::runtime_error("Unmatched parenthesis in JNI function prototype!");
   }
   return result;
  }

 private:
  std::map<char, std::function<callback_t>> callbacks;
  std::vector<T> result;
  std::mutex parserMutex;

  void parseClass(const char *& signature, bool runCallback = true) {
   const char * start = signature;
   while (*signature != ';') {
    signature++;
   }
   auto& callback = callbacks['L'];
   if (callback && runCallback) {
    _JNI_PARSER_TOKEN_CALLBACK
   }
  }

  void parseArray(const char *& signature) {
   const char * start = signature;
   bool loop = true;
   do {
    signature++;
    switch(*signature) {
     case '[': continue;
     case 'L': parseClass(signature, false);
     default: loop = false;
    }
   } while(loop);
   auto& callback = callbacks['['];
   if (callback) {
    _JNI_PARSER_TOKEN_CALLBACK
   }
  }
 };

#undef _JNI_PARSER_TOKEN_CALLBACK
#undef _CASE_CALLBACK_IF_DEFINED
}