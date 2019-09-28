#pragma once

#include "fake-jni/jvm.h"
#include "fake-jni/string.h"

#include <set>

//JNI java/lang/Throwable implementation
namespace FakeJni {
 class JThrowable : public JObject {
 private:
  std::set<JThrowable *> suppressedExceptions;

  JString * message;
  JThrowable * cause;
  JArray<JThrowable *> * suppressed;

 public:
  DEFINE_CLASS_NAME("java/lang/Throwable")

  JThrowable() noexcept;
  JThrowable(JString * message);
  JThrowable(JString * message, JThrowable * cause);
  JThrowable(JThrowable * cause);

  virtual ~JThrowable();

  template<typename T>
  operator T() const;

  void addSuppressed(JThrowable * exception);
  const JThrowable * fillInStackTrace() const;
  const JThrowable * getCause() const;
  const JString * getMessage() const;
  //JArray<StackTraceElement> * getStackTrace();
  const JArray<JThrowable *> * getSuppressed() const;
  const JThrowable * initCause(JThrowable * cause);
  void printStackTrace() const;
  //void printStackTrace(PrintStream * s);
  //void printStackTrace(PrintWriter s);
  //void setStackTrace(JArray<StackTraceElement> * stackTrace)
  //TODO override toString once it's defined in JObject
  //void toString()
 };
}

_DEFINE_JNI_CONVERSION_OPERATOR(FakeJni::JThrowable, jthrowable)

DEFINE_JNI_TYPE(_jthrowable, "java/lang/Throwable")
DECLARE_NATIVE_TYPE(FakeJni::JThrowable)

DECLARE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JThrowable *)

template<typename T>
FakeJni::JThrowable::operator T() const {
 using component_t = typename CX::ComponentTypeResolver<T>::type;
 constexpr const auto
  upcast = __is_base_of(component_t, JThrowable),
  downcast = __is_base_of(JThrowable, component_t),
  jnicast = CX::MatchAny<component_t, _jobject, _jthrowable>::value;
 static_assert(
  upcast || downcast || jnicast,
  "JThrowable can only be upcasted, downcasted or converted to jobject or jthrowable!"
 );
 auto ptr = const_cast<JThrowable *>(this);
 if constexpr(upcast || downcast) {
  return (T&)*ptr;
 } else {
  return CX::union_cast<T>(ptr)();
 }
}