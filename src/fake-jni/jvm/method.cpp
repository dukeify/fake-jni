#include "fake-jni/jvm.h"

#include <cx/tuple.h>

#include <ffi.h>

#include <limits>
#include <map>
#include <functional>

#define SIG_PREAMBLE(case_char, ffi_type) \
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

#define SIG_PARSE_FAILURE \
throw std::runtime_error("FATAL: Could not parse signature for native method: '" + std::string(name) + "'!");

#define RESOLVER_CASE(a_type, ffi_type) \
if (type == &ffi_type) {\
 return CX::Tuple<void (*)(), void (*)(), void (*)()> {\
  (void (*)())&NativeInvocationManager<a_type>::allocate<va_list>,\
  (void (*)())&NativeInvocationManager<a_type>::allocate<jvalue *>,\
  (void (*)())&NativeInvocationManager<a_type>::deallocate\
 };\
}

#define JMETHODID_INVOCATION_PREP \
const auto argc = descriptor->nargs - 2;\
for (unsigned int i = 0; i < argc; i++) {\
 const auto type = descriptor->arg_types[i + 2];\
 const auto typeResolvers = typeResolverGenerator(type);\
 resolvers[i] = typeResolvers.get<0>();\
 resolvers[i + argc] = typeResolvers.get<1>();\
 deallocators[i] = typeResolvers.get<2>();\
}

//TODO find a better name for this glue template
template<typename Arg>
struct NativeInvocationManager {
 template<typename ListType>
 static void * allocate(ListType);

 template<>
 static void * allocate<va_list>(va_list list) {
  return new Arg;
 }

 template<>
 static void * allocate<jvalue *>(jvalue * const values) {
  printf("jvalue * allocate!\n");
  const auto data = new Arg;
  *data = FakeJni::_CX::getAArg<Arg>(values);
  return (void *)data;
 }

 static void deallocate(void * const data) {
  delete (Arg *)data;
 }
};

//Generates the type resolvers, allocators and deallocators for the respective JNI argument list
static auto typeResolverGenerator(ffi_type * const type) {
 RESOLVER_CASE(FakeJni::JBoolean, ffi_type_uint8)
 RESOLVER_CASE(FakeJni::JByte, ffi_type_sint8)
 RESOLVER_CASE(FakeJni::JChar, ffi_type_uint16)
 RESOLVER_CASE(FakeJni::JShort, ffi_type_sint16)
 RESOLVER_CASE(FakeJni::JInt, ffi_type_sint32)
 RESOLVER_CASE(FakeJni::JFloat, ffi_type_float)
 RESOLVER_CASE(FakeJni::JLong, ffi_type_sint64)
 RESOLVER_CASE(FakeJni::JDouble, ffi_type_double)
 RESOLVER_CASE(FakeJni::JObject *, ffi_type_pointer)
 //This should never happen, if it does there's probably memory corruption
 throw std::runtime_error("FATAL: Could not resolve valid FFI type after parsing!");
};

//Non-template members of JMethodID
namespace FakeJni {
 std::map<size_t, std::pair<unsigned long, ffi_cif *>> JMethodID::descriptors;

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

 std::vector<ffi_type *> JMethodID::getFfiPrototype(const char * signature, const char * name) {
  using int_limit_t = std::numeric_limits<unsigned int>;
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
      SIG_PARSE_FAILURE
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
    SIG_PREAMBLE(V, ffi_type_void)
    SIG_PREAMBLE(Z, ffi_type_uint8)
    SIG_PREAMBLE(B, ffi_type_sint8)
    SIG_PREAMBLE(C, ffi_type_uint16)
    SIG_PREAMBLE(S, ffi_type_sint16)
    SIG_PREAMBLE(I, ffi_type_sint32)
    SIG_PREAMBLE(F, ffi_type_float)
    SIG_PREAMBLE(J, ffi_type_sint64)
    SIG_PREAMBLE(D, ffi_type_double)
    case ')': continue;
    default: {
     if (!(inside_array || inside_obj_stmt)) {
      SIG_PARSE_FAILURE
     }
    }
   }
  }
  //No arguments were parsed, not even the return type
  if (argc == int_limit_t::max()) {
   SIG_PARSE_FAILURE
  }
  return ffiTypes;
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
  descriptor([&]() -> ffi_cif * {
   auto& pair = descriptors[std::hash<std::string>{}(signature)];
   pair.first += 1;
   auto& descriptor = pair.second;
   if (!descriptor) {
    auto&& args = getFfiPrototype(signature, name);
    const auto argc = (unsigned int)args.size() - 2;
    descriptor = new ffi_cif;
    auto types = new ffi_type*[argc + 2];
    //env type
    types[0] = &ffi_type_pointer;
    //class or obj
    types[1] = &ffi_type_pointer;
    //the actual function arguments (skip env and obj)
    for (unsigned int i = 0; i < argc; i++) {
     types[i + 2] = args[i];
    }
    auto status = ffi_prep_cif(descriptor, FFI_DEFAULT_ABI, argc + 2, args[argc + 1], types);
    if (status != FFI_OK) {
     throw std::runtime_error(
      "FATAL: ffi_prep_cif failed for function: '"
      + std::string(name)
      + signature
      + "' with error: "
      + std::to_string((uint32_t)status)
      + "!"
     );
    }
   }
   return descriptor;
  }()),
  resolvers(new void_func_t[2 * (descriptor->nargs - 2)]),
  deallocators(new void_func_t[descriptor->nargs - 2]),
  isArbitrary(false)
 {
  JMETHODID_INVOCATION_PREP
 }

 JMethodID::JMethodID(arbitrary_func_t func, const char * signature, const char * name, uint32_t modifiers) :
  _jmethodID(),
  JNINativeMethod {
   verifyName(name),
   verifySignature(signature),
   (void *)func
  },
  //ReturnType (*fnPtr)(JNIEnv *env, jobject objectOrClass, ...)
  type(REGISTER_NATIVES_FUNC),
  descriptor([&]() -> ffi_cif * {
   auto& pair = descriptors[std::hash<std::string>{}(signature)];
   pair.first += 1;
   auto& descriptor = pair.second;
   if (!descriptor) {
    auto&& args = getFfiPrototype(signature, name);
    const auto argc = (unsigned int)args.size() - 2;
    descriptor = new ffi_cif;
    auto types = new ffi_type*[2];
    types[0] = &ffi_type_pointer;
    types[1] = &ffi_type_pointer;
    auto status = ffi_prep_cif_var(descriptor, FFI_DEFAULT_ABI, 2, argc + 2, args[argc + 1], types);
    if (status != FFI_OK) {
     throw std::runtime_error(
      "FATAL: ffi_prep_cif_var failed for function: '"
       + std::string(name)
       + signature
       + "' with error: "
       + std::to_string((uint32_t)status)
       + "!"
     );
    }
   }
   return descriptor;
  }()),
  resolvers(new void_func_t[2 * (descriptor->nargs - 2)]),
  deallocators(new void_func_t[descriptor->nargs - 2]),
  isArbitrary(true)
 {
  JMETHODID_INVOCATION_PREP
 }

 bool JMethodID::operator ==(const JMethodID & mid) const noexcept {
  return (name == mid.name)
   && (strcmp(signature, mid.signature) == 0)
   && (type == mid.type)
   && (modifiers == mid.modifiers)
   && ((type == MEMBER_FUNC) ? (fnPtr == mid.fnPtr && adj == mid.adj) : (fnPtr == mid.fnPtr))
   && (proxyFuncV == mid.proxyFuncV)
   && (proxyFuncA == mid.proxyFuncA);
 }

 bool JMethodID::operator==(JNINativeMethod *& mid) const {
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
   auto& pair = descriptors[std::hash<std::string>{}(signature)];
   pair.first -= 1;
   if (pair.first == 0) {
    auto& cif = pair.second;
    delete[] cif->arg_types;
    delete cif;
   }
  }
 }
}