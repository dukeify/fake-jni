#include "fake_objects.h"

//Example native class with using explicit registration and strictly RAII
class MyNativeClass : public FakeClass {
public:
  //Example static variable
  inline static unsigned long myLong = 42L;
  //Example member variable
  std::string myStr = "Hello World!";

  //Method descriptor for member function
  _jmethodID mid1 {&MyNativeClass::test};
  //Method descriptor for static function
  _jmethodID mid2 {&test2};

  //Field descriptor for member variable
  _jfieldID fid1 {"myStr", &MyNativeClass::myStr};
  //Field descriptor for static variable
  _jfieldID fid2 {"myLong", &myLong};

  MyNativeClass() : FakeClass("com/example/MyNativeClass") {
    //Register example member method
    registerMethod(mid1);
    //Register example static method
    registerMethod(mid2);

    //Register member field descriptor
    registerField(fid1);
    //Register static field descriptor
    registerField(fid2);
  }

  ~MyNativeClass() override = default;

  //Example member function
  _jobject test() {
    std::cout << "This is an instance test!" << std::endl;
    return {};
  }

  //Example static function
  static void test2() {
    std::cout << "This is a static test!" << std::endl;
  }
};

static MyNativeClass myNativeClass;

//NOTE since the JNI requires that pointers be tossed around for value assignment and retrieval, there must be some
// statically allocated variables for the example.
int main() {
  //High-Level API Examples
  //Example member function invocation
  _jobject ret = myNativeClass.invokeMethod<_jobject>(myNativeClass.mid1);
  //Example static function invocation
  myNativeClass.invokeMethod<void>(myNativeClass.mid2);

  //TODO Finish instance member retrieval
  //Example member value retrieval
  //std::string str = *myNativeClass.getField<std::string>(myNativeClass.fid1);
  //std::cout << str << std::endl;

  //Example static value retrieval
  unsigned long l = *myNativeClass.getField<unsigned long>(myNativeClass.fid2);
  std::cout << l << std::endl;

  //Example member value assignment
  //NOTE assignment to fields completely disregards any lifetime modifiers, such as const, and thus will cause an
  // unexpected SIGSEGV when illegally assigning to a const member
  static std::string exampleStr = "yo yo yo";
  myNativeClass.setField(myNativeClass.fid1, exampleStr);
  std::cout << myNativeClass.myStr << std::endl;

  //Example static value assignment
  static unsigned long ul = 3141592653589793238;
  myNativeClass.setField(myNativeClass.fid2, ul);
  std::cout << myNativeClass.myLong << std::endl;

  //Traditional JNI Examples
  //_jmethodID jmid = myNativeClass.jmid;
  //jobject returned = CallObjectMethod(&myNativeClass, &jmid);
  return 0;
}