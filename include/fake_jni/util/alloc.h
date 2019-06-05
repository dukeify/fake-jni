#pragma once

#include "list.h"

#include <mutex>

namespace FakeJVM {
 template<typename T>
 class AllocManager final {
 private:
  //Used as a backward linking
  const List<T> ts;
  const std::mutex ts_lock;

  template<typename... Args>
  explicit AllocManager(Args... args) {

  };

  //Pushes a new allocation to the end of the list
  void pushAlloc(T t) {

  }

  void popAlloc() {

  }

  void removeAlloc(T t) {

  }
 };
}