#include <iostream>

#include "fake-jni/jvm.h"

using namespace FakeJni;

class ExampleClass: public NativeObject<ExampleClass> {
public:
 DEFINE_CLASS_NAME("com/example/ExampleClass")

 JInt exampleField1;
 JString exampleField2;

 //This constructor is visible to the JNI
 ExampleClass():
  NativeObject<ExampleClass>(),
  exampleField1(10),
  exampleField2("Hello World!")
 {}

 //This constructor is visible to the JNI
 ExampleClass(JInt i, JString str):
  NativeObject<ExampleClass>(),
  exampleField1(i),
  exampleField2(str)
 {}

 //This constructor is not visible to the JNI
 ExampleClass(JBooleanArray arr):
  NativeObject<ExampleClass>(),
  exampleField1(-1),
  exampleField2("This constructor has no linked delegate!")
 {}

 JInt* exampleFunction() {
  return &exampleField1;
 }

 JString* getMyString() {
  return &exampleField2;
 }

 inline static void exampleStaticFunction(JDouble d) {
  std::cout << "From ExampleClass: " << d << std::endl;
 }

 static void imNotLinkedToAnything(const char *str) {
  std::cout << str << std::endl;
 }
};

static void outOfLineMemberFunction() {
 std::cout << "I am JVM static!" << std::endl;
}

//Example constructor delegate
static ExampleClass * constructorDelegate() {
 return new ExampleClass();
}

//Another example constructor delegate
static ExampleClass * anotherConstructorDelegate(JInt i, JString *str) {
 return new ExampleClass(i, *str);
}

DEFINE_NATIVE_TYPE(ExampleClass) {
 //Link member functions
 {&ExampleClass::exampleFunction, "exampleFunction"},
 {&ExampleClass::getMyString, "getMyString"},
 //Link static function
 {&exampleStaticFunction, "exampleStaticFunction", JMethodID::STATIC},
 //Link member function
 {&exampleStaticFunction, "theSameFunctionButNotStaticAndWithADifferentName"},
 //Link static function
 {&outOfLineMemberFunction, "woahTheNameIsDifferentAgain"},
 //Register constructor delegates
 {&constructorDelegate},
 {&anotherConstructorDelegate}
};

//fake-jni in action
int main(int argc, char **argv) {
 JString test{"Hello World!"};

 //Create a shiny new fake JVM instance
 DefaultJvm vm;

 //Register ExampleClass on the VM)
 vm.registerClass<ExampleClass>();

 //Attach this binary as a native library
 //no path to current binary, no options, custom library dl functions
// vm.attachLibrary("", "", {&dlopen, &dlsym, &dlclose});

 //Start fake-jni
// vm.start();

 vm.unregisterClass<ExampleClass>();

 //Clean up
 vm.destroy();
 return 0;
}