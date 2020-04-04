#include "fake-jni/throwable.h"

using namespace FakeJni;

JThrowable::JThrowable() noexcept :
 message(JString::EMPTY),
 cause(nullptr)
{}

JThrowable::JThrowable(JString * message) :
 message(message),
 cause(nullptr)
{}

JThrowable::JThrowable(JString * message, JThrowable * cause) :
 message(message),
 cause(cause)
{}

JThrowable::JThrowable(JThrowable * cause) :
 message(nullptr),
 cause(cause)
{}

JThrowable::~JThrowable() {
 if (!suppressed) {
  delete suppressed;
 }
}

void JThrowable::addSuppressed(JThrowable * exception) {
 suppressedExceptions.insert(exception);
}

const JThrowable * JThrowable::fillInStackTrace() const {
 return this;
}

const JThrowable * JThrowable::getCause() const {
 return cause;
}

const JString * JThrowable::getMessage() const {
 return message;
}

const JArray<JThrowable *>* JThrowable::getSuppressed() const {
 const auto size = suppressedExceptions.size();
 if (!suppressed) {
  if (size == (size_t)suppressed->getSize()) {
   return suppressed;
  } else {
   delete suppressed;
  }
 }
 auto ptr = const_cast<JThrowable *>(this);
 ptr->suppressed = new JArray<JThrowable *>(size);
 int i = 0;
 for (auto& exception : suppressedExceptions) {
  (*ptr->suppressed)[i++] = exception;
 }
 return suppressed;
}

const JThrowable * JThrowable::initCause(JThrowable * cause) {
 this->cause = cause;
 return this;
}

void JThrowable::printStackTrace() const {
 fprintf(stderr, "Exception thrown: '%s'\n", getClass().getName());
 const char * c_msg;
 if (message) {
  c_msg = (const char *)message->getArray();
  fprintf(stderr, " - Message: %.*s\n", message->getLength(), c_msg);
 }
 for (auto& exception : suppressedExceptions) {
  fprintf(stderr, "\tSuppressed: '%s'\n", exception->getClass().getName());
 }
 if (cause) {
  fprintf(stderr, "Caused by: '%s'\n", cause->getClass().getName());
  auto& causeMessage = cause->message;
  if (causeMessage) {
   c_msg = (const char *)causeMessage->getArray();
   fprintf(stderr, " - Message: %.*s\n", causeMessage->getLength(), c_msg);
  }
 }
}

DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JThrowable *)

//Allocate JClass descriptor for JThrowable
DEFINE_NATIVE_DESCRIPTOR(JThrowable)
 {Constructor<JThrowable> {}},
 {Constructor<JThrowable, JString *> {}},
 {Constructor<JThrowable, JString *, JThrowable *> {}},
 {Constructor<JThrowable, JThrowable *> {}},
 {Function<&JThrowable::addSuppressed> {}, "addSuppressed"},
 {Function<&JThrowable::fillInStackTrace>{}, "fillInStackTrace"},
 {Function<&JThrowable::getCause>{}, "getCause"},
 {Function<&JThrowable::getMessage>{}, "getMessage"},
 {Function<&JThrowable::getSuppressed>{}, "getSuppressed"},
 {Function<&JThrowable::initCause>{}, "initCause"},
 {Function<&JThrowable::printStackTrace>{}, "printStackTrace"}
END_NATIVE_DESCRIPTOR