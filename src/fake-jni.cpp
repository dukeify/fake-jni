#include <cstring>

#include "fake-jni/fake-jni.h"

#define _CERATE_MAIN_METHOD_CHECK \
auto log = vm.getLog();\
JClass * clazz;\
if (!cclazz) {\
 clazz = createDummyClass(vm);\
} else {\
 clazz = const_cast<JClass *>(cclazz);\
 for (auto mid : clazz->getMethods()) {\
  if (strcmp(mid->getName(), "main") == 0 && strcmp(mid->getSignature(), "([Ljava/lang/String;)V") == 0) {\
   fprintf(log, "WARNING: '%s' already contains a main method definition!\n", clazz->getName());\
   return;\
  }\
 }\
}\
auto& rVm = const_cast<Jvm &>(vm);\
clazz->registerMethod(new JMethodID(std::move(main), "main", JMethodID::PUBLIC | JMethodID::STATIC));\
rVm.registerClass(clazz);

namespace FakeJni {
 JClass * createDummyClass(const Jvm & vm) {
  std::string dummyName = "__Dummy";
  bool nameOverlaps = true;
  while (nameOverlaps) {
   nameOverlaps = false;
   for (auto& clazz : vm.getClasses()) {
    if (strcmp(clazz->getName(), dummyName.c_str()) == 0) {
     dummyName.push_back('_');
     nameOverlaps = true;
     break;
    }
   }
  }
  return new JClass(dummyName.c_str(), JClass::PUBLIC);
 }

 void createMainMethod(const Jvm & vm, _CX::main_method_t * main, JClass * cclazz) {
  _CERATE_MAIN_METHOD_CHECK
 }

 void createMainMethod(const Jvm & vm, CX::Lambda<_CX::main_method_t> main, const JClass * cclazz) {
  _CERATE_MAIN_METHOD_CHECK
 }
}

DEFINE_NATIVE_ARRAY_DESCRIPTOR(FakeJni::JString *)