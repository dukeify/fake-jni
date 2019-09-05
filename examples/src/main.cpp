#include <iostream>

#include "fake-jni/jvm.h"

using namespace FakeJni;

class ExampleClass : public NativeObject<ExampleClass> {
public:
 DEFINE_CLASS_NAME("com/example/ExampleClass")

 using cast = CX::ExplicitCastGenerator<ExampleClass, JClass, JObject>;

 JInt exampleField1;
 JString exampleField2;

 //This constructor is visible to the JNI
 ExampleClass() :
  NativeObject<ExampleClass>(),
  exampleField1(10),
  exampleField2("Hello World!")
 {}

 //This constructor is visible to the JNI
// ExampleClass(JInt i, JString *str) :
//  NativeObject<ExampleClass>(),
//  exampleField1(i),
//  exampleField2(*str)
// {}

 ExampleClass(JInt, JString *) :
  exampleField1(0),
  exampleField2{""}
 {}

 ExampleClass(JDouble, ExampleClass *) :
  exampleField1(0),
  exampleField2{""}
 {}

// //This constructor is not visible to the JNI
// ExampleClass(JBooleanArray arr):
//  NativeObject<ExampleClass>(),
//  exampleField1(-1),
//  exampleField2("This constructor has no linked delegate!")
// {}

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

//TODO #19 the JClass instance needs a reference to the NativeClass somehow
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
 {Constructor<ExampleClass> {}},
// {Constructor<ExampleClass, JInt, JString*> {}}
 //TODO This also needs to be fixed, see TODO document
 {Constructor<ExampleClass, JDouble, ExampleClass *> {}}
};

//fake-jni in action
int main(int argc, char **argv) {
 //Make a JString
 JString test{"Hello World!"};

 //Create a shiny new fake JVM instance
 Jvm * const vm = createJvm();

 //Register ExampleClass on the VM)
 vm->registerClass<ExampleClass>();
// vm->registerClass<TestClass>();

 //Attach this binary as a native library
 //no path to current binary, no options, custom library dl functions
// vm->attachLibrary("", "", {&dlopen, &dlsym, &dlclose});

 //Start fake-jni
// vm->start();

 vm->unregisterClass<ExampleClass>();

 //Clean up
 vm->destroy();

 delete vm;
 return 0;
}