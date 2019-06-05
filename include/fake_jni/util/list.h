#pragma once

namespace FakeJVM {
 template<typename T>
 class List {
 public:
  T t;
  List<T> *next;
 };
}