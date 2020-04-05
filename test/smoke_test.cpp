#include <gtest/gtest.h>

#include <fake-jni/jvm.h>
#include <fake-jni/array.h>
#include <fake-jni/string.h>
#include <fake-jni/throwable.h>

using namespace FakeJni;

class AnimalSmokeTest : public JObject {
public:
    DEFINE_CLASS_NAME("non/existent/path/Animal")

    JInt age_;
    JString name_;

    AnimalSmokeTest(JInt age, JString* name) : age_(age), name_(*name) {
    }

    JInt getAge() {
        return age_;
    }

    JString* getName() {
        return &name_;
    }
};

BEGIN_NATIVE_DESCRIPTOR(AnimalSmokeTest)
    {Field<&AnimalSmokeTest::age_> {}, "age"},
    {Field<&AnimalSmokeTest::name_> {}, "name"},

    {Function<&AnimalSmokeTest::getAge> {}, "getAge"},
    {Function<&AnimalSmokeTest::getName> {}, "getName"},

    {Constructor<AnimalSmokeTest, JInt, JString*> {}},
END_NATIVE_DESCRIPTOR

class SmokeTest : public ::testing::Test {
protected:
    Jvm vm;

    void SetUp() override {
        vm.registerClass<AnimalSmokeTest>();
    }

    void TearDown() override {
        vm.unregisterClass<AnimalSmokeTest>();

        vm.destroy();
    }
};

TEST_F(SmokeTest, CanConstruct) {
    auto& descriptor = AnimalSmokeTest::descriptor;
    auto methodId = descriptor.getMethod("<init>", "(ILjava.lang.String;)V");
    JString name = "Duke";
    methodId->invoke(&vm, &descriptor, 23, &name);
}