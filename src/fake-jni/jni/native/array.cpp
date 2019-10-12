#include "fake-jni/jvm.h"
#include "fake-jni/array.h"

#include <cx/unsafe.h>

#include <stdexcept>

#define _NEW_ARRAY(array_type)\
auto arr = new array_type(size);\
vm.addInstance(arr);\
return *arr;

#define _GET_ARRAY_ELEMENTS(array_type)\
auto arr = CX::union_cast<array_type *>(jarr);\
if (copy) {\
 if (*copy) {\
  const auto len = arr->getSize();\
  auto data = new typename array_type::component[len];\
  for (JInt i = 0; i < len; i++) {\
   data[i] = (*arr)[i];\
  }\
  return data;\
 }\
}\
return arr->getArray();

//TODO ensure that a matching getArrayElements invocation occurred before freeing
#define _FREE_ARRAY_ELEMENTS(array_type)\
auto arr = CX::union_cast<array_type *>(jarr);\
const auto len = arr->getSize();\
if (mode == 0) {\
 for (JInt i = 0; i < len; i++) {\
  (*arr)[i] = elems[i];\
 }\
} else if (mode == JNI_COMMIT) {\
 for (JInt i = 0; i < len; i++) {\
  (*arr)[i] = elems[i];\
 }\
 delete[] elems;\
} else if (mode == JNI_ABORT) {\
 delete[] elems;\
} else {\
 throw std::runtime_error("FATAL: Invalid mode specified for release array elements!");\
}

//TODO JNI exception compliance
#define _GET_ARRAY_REGION(array_type)\
auto arr = CX::union_cast<array_type *>(jarr);\
const auto size = arr->getSize();\
if (0 > len || start + len > size) {\
 throw std::runtime_error("FATAL: Invalid array region requested!");\
}\
for (JInt i = start; i < start + len; i++) {\
 buf[i] = (*arr)[i];\
}

#define _SET_ARRAY_REGION(array_type)\
auto arr = CX::union_cast<array_type *>(jarr);\
const auto size = arr->getSize();\
if (0 > len || start + len > size) {\
 throw std::runtime_error("FATAL: Invalid array region requested!");\
}\
for (JInt i = start; i < start + len; i++) {\
 (*arr)[i] = buf[i];\
}

namespace FakeJni {
 //TODO implement
 void* NativeInterface::getPrimitiveArrayCritical(jarray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getPrimitiveArrayCritical' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::releasePrimitiveArrayCritical(jarray, void *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releasePrimitiveArrayCritical' is unimplemented!");
 }

 jsize NativeInterface::getArrayLength(jarray jarr) const {
  return CX::union_cast<JArrayBase *>(jarr)->getSize();
 }

 jobjectArray NativeInterface::newObjectArray(jsize size, jclass element_t, jobject initialElement) const {
  auto arr = new JObjectArray(size);
  for (JInt i = 0; i < size; i++) {
   arr[i] = *CX::union_cast<JObject *>(initialElement);
  }
  vm.addInstance(arr);
  return *arr;
 }

 jobject NativeInterface::getObjectArrayElement(jobjectArray arr, jsize index) const {
  return (*CX::union_cast<JObjectArray *>(arr))[index];
 }

 void NativeInterface::setObjectArrayElement(jobjectArray jarr, jsize index, jobject obj) const {
  (*CX::union_cast<JObjectArray *>(jarr))[index] = *CX::union_cast<JObject *>(obj);
 }

 jbooleanArray NativeInterface::newBooleanArray(jsize size) const {
  _NEW_ARRAY(JBooleanArray)
 }

 jbyteArray NativeInterface::newByteArray(jsize size) const {
  _NEW_ARRAY(JByteArray)
 }

 jcharArray NativeInterface::newCharArray(jsize size) const {
  _NEW_ARRAY(JCharArray)
 }

 jshortArray NativeInterface::newShortArray(jsize size) const {
  _NEW_ARRAY(JShortArray)
 }

 jintArray NativeInterface::newIntArray(jsize size) const {
  _NEW_ARRAY(JIntArray)
 }

 jlongArray NativeInterface::newLongArray(jsize size) const {
  _NEW_ARRAY(JLongArray)
 }

 jfloatArray NativeInterface::newFloatArray(jsize size) const {
  _NEW_ARRAY(JFloatArray)
 }

 jdoubleArray NativeInterface::newDoubleArray(jsize size) const {
  _NEW_ARRAY(JDoubleArray)
 }

 jboolean * NativeInterface::getBooleanArrayElements(jbooleanArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JBooleanArray)
 }

 jbyte * NativeInterface::getByteArrayElements(jbyteArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JByteArray)
 }

 jchar * NativeInterface::getCharArrayElements(jcharArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JCharArray)
 }

 jshort * NativeInterface::getShortArrayElements(jshortArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JShortArray)
 }

 jint * NativeInterface::getIntArrayElements(jintArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JIntArray)
 }

 jlong * NativeInterface::getLongArrayElements(jlongArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JLongArray)
 }

 jfloat * NativeInterface::getFloatArrayElements(jfloatArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JFloatArray)
 }

 jdouble * NativeInterface::getDoubleArrayElements(jdoubleArray jarr, jboolean * copy) const {
  _GET_ARRAY_ELEMENTS(JDoubleArray)
 }

 void NativeInterface::releaseBooleanArrayElements(jbooleanArray jarr, jboolean * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JBooleanArray)
 }

 void NativeInterface::releaseByteArrayElements(jbyteArray jarr, jbyte * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JByteArray)
 }

 void NativeInterface::releaseCharArrayElements(jcharArray jarr, jchar * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JCharArray)
 }

 void NativeInterface::releaseShortArrayElements(jshortArray jarr, jshort * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JShortArray)
 }

 void NativeInterface::releaseIntArrayElements(jintArray jarr, jint * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JIntArray)
 }

 void NativeInterface::releaseLongArrayElements(jlongArray jarr, jlong * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JLongArray)
 }

 void NativeInterface::releaseFloatArrayElements(jfloatArray jarr, jfloat * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JFloatArray)
 }

 void NativeInterface::releaseDoubleArrayElements(jdoubleArray jarr, jdouble * elems, jint mode) const {
  _FREE_ARRAY_ELEMENTS(JDoubleArray)
 }

 void NativeInterface::getBooleanArrayRegion(jbooleanArray jarr, jsize start, jsize len, jboolean * buf) const {
  _GET_ARRAY_REGION(JBooleanArray)
 }

 void NativeInterface::getByteArrayRegion(jbyteArray jarr, jsize start, jsize len, jbyte * buf) const {
  _GET_ARRAY_REGION(JByteArray)
 }

 void NativeInterface::getCharArrayRegion(jcharArray jarr, jsize start, jsize len, jchar * buf) const {
  _GET_ARRAY_REGION(JCharArray)
 }

 void NativeInterface::getShortArrayRegion(jshortArray jarr, jsize start, jsize len, jshort * buf) const {
  _GET_ARRAY_REGION(JShortArray)
 }

 void NativeInterface::getIntArrayRegion(jintArray jarr, jsize start, jsize len, jint * buf) const {
  _GET_ARRAY_REGION(JIntArray)
 }

 void NativeInterface::getLongArrayRegion(jlongArray jarr, jsize start, jsize len, jlong * buf) const {
  _GET_ARRAY_REGION(JLongArray)
 }

 void NativeInterface::getFloatArrayRegion(jfloatArray jarr, jsize start, jsize len, jfloat * buf) const {
  _GET_ARRAY_REGION(JFloatArray)
 }

 void NativeInterface::getDoubleArrayRegion(jdoubleArray jarr, jsize start, jsize len, jdouble * buf) const {
  _GET_ARRAY_REGION(JDoubleArray)
 }

 void NativeInterface::setBooleanArrayRegion(jbooleanArray jarr, jsize start, jsize len, const jboolean * buf) const {
  _SET_ARRAY_REGION(JBooleanArray)
 }

 void NativeInterface::setByteArrayRegion(jbyteArray jarr, jsize start, jsize len, const jbyte * buf) const {
  _SET_ARRAY_REGION(JByteArray)
 }

 void NativeInterface::setCharArrayRegion(jcharArray jarr, jsize start, jsize len, const jchar * buf) const {
  _SET_ARRAY_REGION(JCharArray)
 }

 void NativeInterface::setShortArrayRegion(jshortArray jarr, jsize start, jsize len, const jshort * buf) const {
  _SET_ARRAY_REGION(JShortArray)
 }

 void NativeInterface::setIntArrayRegion(jintArray jarr, jsize start, jsize len, const jint * buf) const {
  _SET_ARRAY_REGION(JIntArray)
 }

 void NativeInterface::setLongArrayRegion(jlongArray jarr, jsize start, jsize len, const jlong * buf) const {
  _SET_ARRAY_REGION(JLongArray)
 }

 void NativeInterface::setFloatArrayRegion(jfloatArray jarr, jsize start, jsize len, const jfloat * buf) const {
  _SET_ARRAY_REGION(JFloatArray)
 }

 void NativeInterface::setDoubleArrayRegion(jdoubleArray jarr, jsize start, jsize len, const jdouble * buf) const {
  _SET_ARRAY_REGION(JDoubleArray)
 }
}