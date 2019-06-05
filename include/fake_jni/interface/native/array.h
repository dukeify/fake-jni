#pragma once

namespace FakeJVM {
 //TODO implement
 void* NativeInterface::getPrimitiveArrayCritical(jarray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getPrimitiveArrayCritical' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::releasePrimitiveArrayCritical(jarray, void *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releasePrimitiveArrayCritical' is unimplemented!");
 }

//TODO implement
 jsize NativeInterface::getArrayLength(jarray) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getArrayLength' is unimplemented!");
  return 0;
 }

//TODO implement
 jobjectArray NativeInterface::newObjectArray(jsize, jclass, jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newObjectArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jobject NativeInterface::getObjectArrayElement(jobjectArray, jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getObjectArrayElement' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::setObjectArrayElement(jobjectArray, jsize, jobject) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setObjectArrayElement' is unimplemented!");
 }

//TODO implement
 jbooleanArray NativeInterface::newBooleanArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newBooleanArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jbyteArray NativeInterface::newByteArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newByteArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jcharArray NativeInterface::newCharArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newCharArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jshortArray NativeInterface::newShortArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newShortArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jintArray NativeInterface::newIntArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newIntArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jlongArray NativeInterface::newLongArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newLongArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jfloatArray NativeInterface::newFloatArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newFloatArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jdoubleArray NativeInterface::newDoubleArray(jsize) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::newDoubleArray' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jboolean* NativeInterface::getBooleanArrayElements(jbooleanArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getBooleanArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jbyte* NativeInterface::getByteArrayElements(jbyteArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getByteArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jchar* NativeInterface::getCharArrayElements(jcharArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getCharArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jshort* NativeInterface::getShortArrayElements(jshortArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getShortArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jint* NativeInterface::getIntArrayElements(jintArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getIntArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jlong* NativeInterface::getLongArrayElements(jlongArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getLongArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jfloat* NativeInterface::getFloatArrayElements(jfloatArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getFloatArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 jdouble* NativeInterface::getDoubleArrayElements(jdoubleArray, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getDoubleArrayElements' is unimplemented!");
  return nullptr;
 }

//TODO implement
 void NativeInterface::releaseBooleanArrayElements(jbooleanArray, jboolean *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseBooleanArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseByteArrayElements(jbyteArray, jbyte *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseByteArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseCharArrayElements(jcharArray, jchar *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseCharArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseShortArrayElements(jshortArray, jshort *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseShortArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseIntArrayElements(jintArray, jint *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseIntArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseLongArrayElements(jlongArray, jlong *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseLongArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseFloatArrayElements(jfloatArray, jfloat *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseFloatArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::releaseDoubleArrayElements(jdoubleArray, jdouble *, jint) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::releaseDoubleArrayElements' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getBooleanArrayRegion(jbooleanArray, jsize, jsize, jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getBooleanArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getByteArrayRegion(jbyteArray, jsize, jsize, jbyte *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getByteArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getCharArrayRegion(jcharArray, jsize, jsize, jchar *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getCharArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getShortArrayRegion(jshortArray, jsize, jsize, jshort *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getShortArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getIntArrayRegion(jintArray, jsize, jsize, jint *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getIntArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getLongArrayRegion(jlongArray, jsize, jsize, jlong *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getLongArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getFloatArrayRegion(jfloatArray, jsize, jsize, jfloat *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getFloatArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::getDoubleArrayRegion(jdoubleArray, jsize, jsize, jdouble *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::getDoubleArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setBooleanArrayRegion(jbooleanArray, jsize, jsize, const jboolean *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setBooleanArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setByteArrayRegion(jbyteArray, jsize, jsize, const jbyte *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setByteArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setCharArrayRegion(jcharArray, jsize, jsize, const jchar *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setCharArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setShortArrayRegion(jshortArray, jsize, jsize, const jshort *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setShortArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setIntArrayRegion(jintArray, jsize, jsize, const jint *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setIntArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setLongArrayRegion(jlongArray, jsize, jsize, const jlong *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setLongArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setFloatArrayRegion(jfloatArray, jsize, jsize, const jfloat *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setFloatArrayRegion' is unimplemented!");
 }

//TODO implement
 void NativeInterface::setDoubleArrayRegion(jdoubleArray, jsize, jsize, const jdouble *) const {
  throw std::runtime_error("FATAL: 'JVMNativeInterface_::setDoubleArrayRegion' is unimplemented!");
 }
}