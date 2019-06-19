#pragma once

namespace FakeJni {
 namespace _CX {
  template<typename>
  class RegistrationHookGenerator;

  class ClassDescriptorElement {
  public:
   enum DescriptorType {
    MEMBER_FUNCTION,
    NON_MEMBER_FUNCTION,
    MEMBER_FIELD,
    NON_MEMBER_FIELD
   };

   const DescriptorType type;

   union {
    void (AnyClass::* const memberFunc)();
    void (* const nonMemberFunc)();
    int (AnyClass::* const memberField);
    void (* const nonMemberField);
   };

   const char *const name;
   const uint32_t modifiers;
   bool (* const processHook)(JClass *const, ClassDescriptorElement *const);

   //Constructor for member functions
   template<typename R, typename T, typename... Args>
   ClassDescriptorElement(R (T::* const func)(Args...), const char *const name, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    type(MEMBER_FUNCTION),
    memberFunc((decltype(memberFunc))func),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(func)>::process)
   {}

   //Constructor for non-member functions
   template<typename R, typename... Args>
   ClassDescriptorElement(R (*const func)(Args...), const char *const name, uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    type(NON_MEMBER_FUNCTION),
    nonMemberFunc((decltype(nonMemberFunc))func),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(func)>::process)
   {}

   //Constructor for constructor delegates
   template<typename R, typename... Args>
   ClassDescriptorElement(R (*const func)(Args...), uint32_t modifiers = JMethodID::PUBLIC) noexcept :
    ClassDescriptorElement(func, "<init>", modifiers)
   {}

   //Constructor for member fields
   template<typename F, typename T>
   ClassDescriptorElement(F (T::* const field), const char *const name, uint32_t modifiers = JFieldID::PUBLIC) noexcept :
    type(MEMBER_FIELD),
    memberField((decltype(memberField))field),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(field)>::process)
   {}

   //Constructor for non-member fields
   template<typename F>
   ClassDescriptorElement(F *const field, const char *const name, uint32_t modifiers = JFieldID::PUBLIC) noexcept :
    type(NON_MEMBER_FIELD),
    nonMemberField((decltype(nonMemberField))field),
    name(name),
    modifiers(modifiers),
    processHook(&RegistrationHookGenerator<decltype(field)>::process) {}

   [[gnu::always_inline]]
   inline bool process(JClass *const clazz) const {
    return processHook(clazz, const_cast<ClassDescriptorElement *>(this));
   }
  };
 }

 //TODO register java.lang.Class functions for JClass if they are ever required
 class JClass : public JObject, _jclass {
 private:
  static const JClass jClassDescriptor;

  const JClass *copyConstructed;

  //Constructor for JClass descriptor
  JClass(std::nullptr_t) noexcept :
   JObject(),
   copyConstructed(nullptr),
   functions(nullptr),
   fields(nullptr) {}

 protected:

  //Property associations
  AllocStack<JMethodID *> *functions;
  AllocStack<JFieldID *> *fields;

  //TODO
  constexpr bool isNameCompliant(const char *name) const noexcept {
   return true;
  }

 public:
  //TODO ensure that these are correct
  //CLASS_MODIFIERS = 3103;
  //INTERFACE_MODIFIERS = 3087;
  enum Modifiers : uint32_t {
   //TODO
  };

  explicit JClass(JClass &) = delete;
  explicit JClass(JClass &&) = delete;

  JClass(std::initializer_list<_CX::ClassDescriptorElement> list) noexcept :
   JObject(),
   copyConstructed(nullptr),
   functions(new AllocStack<JMethodID *>(true)),
   fields(new AllocStack<JFieldID *>(true))
  {
   for (auto &d : list) {
    d.process(this);
   }
  }

  JClass(const JClass * const clazz) noexcept :
   JObject(),
   copyConstructed(clazz),
   functions(clazz->functions),
   fields(clazz->fields)
  {}

  virtual ~JClass() {
   //The default class descriptor for JClass does not allocate any memory
   if (strcmp(getName(), "java/lang/Class") == 0 && !copyConstructed) {
    delete functions;
    delete fields;
   }
  }

  bool registerMethod(JMethodID *const mid) noexcept {
   for (uint32_t i = 0; i < functions->getSize(); i++) {
    if (*(*functions)[i] == *mid) {
     return false;
    }
   }
   functions->pushAlloc(mid);
   return true;
  }

  bool registerField(JFieldID *const fid) noexcept {
   for (uint32_t i = 0; i < fields->getSize(); i++) {
    if (*(*fields)[i] == *fid) {
     return false;
    }
   }
   fields->pushAlloc(fid);
   return true;
  }

  const JClass *getClass() noexcept override {
   return const_cast<JClass *>(&jClassDescriptor);
  }

  virtual const char *getName() noexcept {
   return "java/lang/Class";
  }

  //By default, allocations will not be supported. This prevents attempts to instantiate
  //`java/lang/Class` directly.
  virtual JObject *newInstance(JavaVM *const vm, const char *signature, va_list list) const {
   return nullptr;
  }
 };

 DEFINE_JNI_TYPE(JClass, Ljava/lang/Class;)

 inline const JClass JClass::jClassDescriptor{nullptr};

 namespace _CX {
  //Generator for member functions
  template<typename R, typename T, typename... Args>
  class RegistrationHookGenerator<R (T::* const)(Args...)> {
  public:
   using type = R (T::* const)(Args...);

   [[gnu::always_inline]]
   inline static bool process(JClass *const clazz, ClassDescriptorElement *const elem) {
    return clazz->registerMethod(new JMethodID((type)elem->memberFunc, elem->name, elem->modifiers));
   }
  };

  //Generator for non-member functions
  template<typename R, typename... Args>
  class RegistrationHookGenerator<R (*const)(Args...)> {
  public:
   using type = R (*const)(Args...);

   [[gnu::always_inline]]
   inline static bool process(JClass *const clazz, ClassDescriptorElement *const elem) {
    if (strcmp(elem->name, "<init>") == 0) {
     //Register constructor
     return clazz->registerMethod(new JMethodID((type)elem->nonMemberFunc, elem->modifiers));
    } else {
     return clazz->registerMethod(new JMethodID((type)elem->nonMemberFunc, elem->name, elem->modifiers));
    }
   }
  };

  //Generator for member fields
  template<typename F, typename T>
  class RegistrationHookGenerator<F (T::* const)> {
  public:
   using type = F (T::* const);

   [[gnu::always_inline]]
   inline static bool process(JClass *const clazz, ClassDescriptorElement *const elem) {
    return clazz->registerField(new JFieldID((type)elem->memberField, elem->name, elem->modifiers));
   }
  };

  //Generator for non-member fields
  template<typename F>
  class RegistrationHookGenerator<F *const> {
  public:
   using type = F *const;

   [[gnu::always_inline]]
   inline static bool process(JClass *const clazz, ClassDescriptorElement *const elem) {
    return clazz->registerField(new JFieldID((type)elem->nonMemberField, elem->name, elem->modifiers));
   }
  };
 }
}