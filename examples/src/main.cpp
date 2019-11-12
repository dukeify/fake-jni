#include <iostream>

#include "fake-jni/jvm.h"
#include "fake-jni/array.h"
#include "fake-jni/string.h"
#include "fake-jni/throwable.h"

using namespace FakeJni;

class ExampleClass : public JObject {
public:
 DEFINE_CLASS_NAME("com/example/ExampleClass")

 JInt exampleField1;
 JString exampleField2;

 //This constructor is visible to the JVM
 ExampleClass() :
  exampleField1(10),
  exampleField2("Hello World!")
 {}

 //This constructor is visible to the JVM
 ExampleClass(JInt, JString *) :
  exampleField1(0),
  exampleField2{""}
 {}

 //This constructor is visible to the JVM
 ExampleClass(JDouble, ExampleClass *) :
  exampleField1(0),
  exampleField2{""}
 {}

 //This constructor is not visible to the JVM and thus no compile-time error is generated
 //for the illegal constructor prototype
 ExampleClass(JBooleanArray arr):
  exampleField1(-1),
  exampleField2("This constructor is not registered on the JVM!")
 {}

 JInt exampleFunction() {
  return exampleField1;
 }

 JString* getMyString() {
  return &exampleField2;
 }

 inline static void exampleStaticFunction(JDouble d) {
  std::cout << "From ExampleClass: " << d << std::endl;
 }

 static void imNotLinkedToAnything(const char * str) {
  std::cout << str << std::endl;
 }
};

static void outOfLineMemberFunction() {
 std::cout << "I am JVM static!" << std::endl;
}

BEGIN_NATIVE_DESCRIPTOR(ExampleClass)
 //Link member functions
 {Function<&ExampleClass::exampleFunction> {}, "exampleFunction"},
 {Function<&ExampleClass::getMyString> {}, "getMyString"},
 //Link static function
 {Function<&exampleStaticFunction> {}, "exampleStaticFunction", JMethodID::STATIC},
 //Link member function
 {Function<&exampleStaticFunction> {}, "theSameFunctionButNotStaticAndWithADifferentName"},
 //Link static function
 {Function<&outOfLineMemberFunction> {}, "woahTheNameIsDifferentAgain"},
 //Link member fields
 {Field<&ExampleClass::exampleField1> {}, "exampleField1"},
 {Field<&ExampleClass::exampleField2> {}, "exampleField2"},
 //Register constructors
 {Constructor<ExampleClass> {}},
 {Constructor<ExampleClass, JInt, JString*> {}},
 {Constructor<ExampleClass, JDouble, ExampleClass *> {}}
END_NATIVE_DESCRIPTOR

static void bruh(JNIEnv *env, jobject clazzOrInst, JDouble value) {
 printf("Value: %f\n", value);
}

//fake-jni in action
int main(int argc, char **argv) {
 //Make a JString
 JString test{"Hello World!"};

 //Create a shiny new fake JVM instance
 Jvm vm;
// vm.registerDefaultSignalHandler();
 vm.start();

 //Register ExampleClass on the JVM instance
 vm.registerClass<ExampleClass>();

 //Attach this binary as a native library
 //no path to current binary, no options, no custom library dl functions
 vm.attachLibrary("");

 //Start fake-jni
// vm->start();

 //Not necessary
 vm.unregisterClass<ExampleClass>();

 //Clean up, also not necessary
 vm.destroy();

 return 0;
}
