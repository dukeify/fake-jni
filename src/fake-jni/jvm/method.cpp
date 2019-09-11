#include "jni.h"

#include "fake-jni/jvm.h"

#include "cx/tuple.h"

#include <ffi.h>

#include <limits>

#define _SIG_PREAMBLE(case_char, ffi_type) \
case #case_char[0]: {\
 if (inside_obj_stmt) continue;\
 if (inside_array) {\
  inside_array = false;\
  argc += 1;\
  ffiTypes.push_back(&ffi_type_pointer);\
  break;\
 }\
}\
argc += 1;\
ffiTypes.push_back(&ffi_type);\
break;

#define _SIG_PARSE_FAILURE \
throw std::runtime_error("FATAL: Could not parse signature for native method: '" + std::string(name) + "'!");

#define RESOLVER_CASE(a_type, ffi_type) \
if (type == &ffi_type) {\
 return CX::Tuple<void (*)(), void (*)(), void (*)()> {\
  (void (*)())&NativeInvocationManager<a_type>::allocate<va_list>,\
  (void (*)())&NativeInvocationManager<a_type>::allocate<jvalue *>,\
  (void (*)())&NativeInvocationManager<a_type>::deallocate\
 };\
}

//TODO find a better name for this glue template
template<typename Arg>
struct NativeInvocationManager {
 template<typename ListType>
 static void * allocate(ListType);

 template<>
 static void * allocate<va_list>(va_list list) {
  const auto data = new Arg;
  *data = FakeJni::_CX::getVArg<Arg>(list);
  return (void *)data;
 }

 template<>
 static void * allocate<jvalue *>(jvalue * const values) {
  const auto data = new Arg;
  *data = FakeJni::_CX::getAArg<Arg>(values);
  return (void *)data;
 }

 static void deallocate(void * const data) {
  delete (Arg *)data;
 }
};

//Non-template members of JMethodID
namespace FakeJni {
 template<>
 inline JMethodID::static_func_t JMethodID::getFunctionProxy<const jvalue *>() const {
  return proxyFuncA;
 }

 //TODO
 char * JMethodID::verifyName(const char * name) {
  return const_cast<char *>(name);
 }

 //TODO
 char * JMethodID::verifySignature(const char * sig) {
  return const_cast<char *>(sig);
 }

 ffi_cif * JMethodID::getFfiPrototype(const char * signature, const char * name) {
  using int_limit_t = std::numeric_limits<unsigned int>;
  static ffi_cif * descriptor = nullptr;
  //only parse shorty if the ffi prototype has not already been generated
  if (!descriptor) {
   descriptor = new ffi_cif;
   bool
    inside_array = false,
    inside_obj_stmt = false;
   auto argc = int_limit_t::max();
   std::vector<ffi_type *> ffiTypes;
   for (char s : std::string(signature)) {
    switch (s) {
     case '(': continue;
     case 'L': {
      inside_obj_stmt = true;
      break;
     }
     case ';': {
      if (!inside_obj_stmt) {
       _SIG_PARSE_FAILURE
      }
      inside_obj_stmt = false;
      if (inside_array) {
       inside_array = false;
      }
      argc += 1;
      ffiTypes.push_back(&ffi_type_pointer);
      break;
     }
     case '[': {
      inside_array = true;
      break;
     }
     _SIG_PREAMBLE(V, ffi_type_void)
     _SIG_PREAMBLE(Z, ffi_type_uint8)
     _SIG_PREAMBLE(B, ffi_type_sint8)
     _SIG_PREAMBLE(C, ffi_type_uint16)
     _SIG_PREAMBLE(S, ffi_type_sint16)
     _SIG_PREAMBLE(I, ffi_type_sint32)
     _SIG_PREAMBLE(F, ffi_type_float)
     _SIG_PREAMBLE(J, ffi_type_sint64)
     _SIG_PREAMBLE(D, ffi_type_double)
     case ')': continue;
     default: _SIG_PARSE_FAILURE
    }
   }
   //No arguments were parsed, not even the return type
   if (argc == int_limit_t::max()) {
    _SIG_PARSE_FAILURE
   }
   auto types = new ffi_type*[argc + 2];
   //env type
   types[0] = &ffi_type_pointer;
   //class or obj
   types[1] = &ffi_type_pointer;
   //the actual function arguments (skip env and obj)
   for (unsigned int i = 0; i < argc; i++) {
    types[i + 2] = ffiTypes[i];
   }
   if (ffi_prep_cif(descriptor, FFI_DEFAULT_ABI, argc + 2, ffiTypes[argc], types) != FFI_OK) {
    throw std::runtime_error("FATAL: ffi_prep_cif failed for function: '" + std::string(name) + "'!");
   }
  }
  return descriptor;
 }

 JMethodID::JMethodID(const JNINativeMethod * method) :
  _jmethodID(),
  JNINativeMethod {
   verifyName(method->name),
   verifySignature(method->signature),
   method->fnPtr
  },
  //ReturnType (*fnPtr)(JNIEnv *env, jobject objectOrClass, ...)
  type(REGISTER_NATIVES_FUNC),
  descriptor(getFfiPrototype(signature, name)),
  resolvers(new void_func_t[2 * (descriptor->nargs - 2)]),
  deallocators(new void_func_t[descriptor->nargs - 2])
 {
  const auto argc = descriptor->nargs - 2;
  //Generates the type resolvers, allocators and deallocators for the respective JNI argument list
  const auto typeResolverGenerator = [](ffi_type * const type) -> auto {
   RESOLVER_CASE(JBoolean, ffi_type_uint8)
   RESOLVER_CASE(JByte, ffi_type_sint8)
   RESOLVER_CASE(JChar, ffi_type_uint16)
   RESOLVER_CASE(JShort, ffi_type_sint16)
   RESOLVER_CASE(JInt, ffi_type_sint32)
   RESOLVER_CASE(JFloat, ffi_type_float)
   RESOLVER_CASE(JLong, ffi_type_sint64)
   RESOLVER_CASE(JDouble, ffi_type_double)
   RESOLVER_CASE(JObject *, ffi_type_pointer)
   //This should never happen, if it does there's probably memory corruption
   throw std::runtime_error("FATAL: Could not resolve valid FFI type after parsing!");
  };
  for (unsigned int i = 0; i < argc; i++) {
   const auto type = descriptor->arg_types[i + 2];
   const auto typeResolvers = typeResolverGenerator(type);
   resolvers[i] = typeResolvers.get<0>(); //va_list resolver
   resolvers[i + argc] = typeResolvers.get<1>(); //jvalue * resolver
   deallocators[i] = typeResolvers.get<2>(); //deallocator
  }
 }

 bool JMethodID::operator ==(JMethodID & mid) const noexcept {
  return (name == mid.name)
   && (strcmp(signature, mid.signature) == 0)
   && (type == mid.type)
   && (modifiers == mid.modifiers)
   && (type ? (memberFunc == mid.memberFunc) : (staticFunc == mid.staticFunc))
   && (proxyFuncV == mid.proxyFuncV)
   && (proxyFuncA == mid.proxyFuncA);
 }

 bool JMethodID::operator ==(JNINativeMethod *& mid) const {
  return modifiers == 0
   && type == REGISTER_NATIVES_FUNC
   && strcmp(name, mid->name) == 0
   && strcmp(signature, mid->signature) == 0
   && fnPtr == mid->fnPtr;
 }

 JMethodID::~JMethodID() {
  //Clean up runtime-generated ffi descriptor and proxy functions
  if (type == REGISTER_NATIVES_FUNC) {
   delete[] resolvers;
   delete[] deallocators;
   const auto cif = getFfiPrototype(signature, name);
   delete[] cif->arg_types;
   delete cif;
  }
 }
}