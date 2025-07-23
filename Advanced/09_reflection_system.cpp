#include <iostream>
#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <memory>
#include <vector>
#include <string>
#include <any>
#include <type_traits>
using namespace std;

class TypeInfo {
private:
    string name;
    size_t size;
    const type_info* typeInfo;
    
public:
    TypeInfo(const string& n, size_t s, const type_info& ti) 
        : name(n), size(s), typeInfo(&ti) {}
    
    const string& getName() const { return name; }
    size_t getSize() const { return size; }
    const type_info& getTypeInfo() const { return *typeInfo; }
    
    bool operator==(const TypeInfo& other) const {
        return *typeInfo == other.getTypeInfo();
    }
};

class Property {
private:
    string name;
    TypeInfo type;
    function<any(const void*)> getter;
    function<void(void*, const any&)> setter;
    
public:
    Property(const string& n, const TypeInfo& t,
             function<any(const void*)> g,
             function<void(void*, const any&)> s)
        : name(n), type(t), getter(g), setter(s) {}
    
    const string& getName() const { return name; }
    const TypeInfo& getType() const { return type; }
    
    any getValue(const void* obj) const {
        return getter(obj);
    }
    
    void setValue(void* obj, const any& value) const {
        setter(obj, value);
    }
};

class Method {
private:
    string name;
    TypeInfo returnType;
    vector<TypeInfo> paramTypes;
    function<any(void*, const vector<any>&)> invoker;
    
public:
    Method(const string& n, const TypeInfo& rt, 
           const vector<TypeInfo>& pt,
           function<any(void*, const vector<any>&)> inv)
        : name(n), returnType(rt), paramTypes(pt), invoker(inv) {}
    
    const string& getName() const { return name; }
    const TypeInfo& getReturnType() const { return returnType; }
    const vector<TypeInfo>& getParameterTypes() const { return paramTypes; }
    
    any invoke(void* obj, const vector<any>& params) const {
        return invoker(obj, params);
    }
};

class ClassInfo {
private:
    string name;
    unordered_map<string, unique_ptr<Property>> properties;
    unordered_map<string, unique_ptr<Method>> methods;
    function<unique_ptr<void, void(*)(void*)>()> constructor;
    
public:
    ClassInfo(const string& n) : name(n) {}
    
    const string& getName() const { return name; }
    
    void addProperty(unique_ptr<Property> prop) {
        properties[prop->getName()] = move(prop);
    }
    
    void addMethod(unique_ptr<Method> method) {
        methods[method->getName()] = move(method);
    }
    
    void setConstructor(function<unique_ptr<void, void(*)(void*)>()> ctor) {
        constructor = ctor;
    }
    
    const Property* getProperty(const string& name) const {
        auto it = properties.find(name);
        return it != properties.end() ? it->second.get() : nullptr;
    }
    
    const Method* getMethod(const string& name) const {
        auto it = methods.find(name);
        return it != methods.end() ? it->second.get() : nullptr;
    }
    
    vector<string> getPropertyNames() const {
        vector<string> names;
        for (const auto& pair : properties) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    vector<string> getMethodNames() const {
        vector<string> names;
        for (const auto& pair : methods) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    unique_ptr<void, void(*)(void*)> createInstance() const {
        return constructor ? constructor() : nullptr;
    }
};

class ReflectionRegistry {
private:
    static unordered_map<string, unique_ptr<ClassInfo>> classes;
    
public:
    static void registerClass(unique_ptr<ClassInfo> classInfo) {
        classes[classInfo->getName()] = move(classInfo);
    }
    
    static const ClassInfo* getClass(const string& name) {
        auto it = classes.find(name);
        return it != classes.end() ? it->second.get() : nullptr;
    }
    
    static vector<string> getAllClassNames() {
        vector<string> names;
        for (const auto& pair : classes) {
            names.push_back(pair.first);
        }
        return names;
    }
};

unordered_map<string, unique_ptr<ClassInfo>> ReflectionRegistry::classes;

template<typename T>
TypeInfo makeTypeInfo() {
    return TypeInfo(typeid(T).name(), sizeof(T), typeid(T));
}

template<typename T>
TypeInfo makeTypeInfo(const string& name) {
    return TypeInfo(name, sizeof(T), typeid(T));
}

#define REFLECT_CLASS(ClassName) \
    class ClassName##Reflection { \
    public: \
        static void registerReflection(); \
    }; \
    namespace { \
        struct ClassName##ReflectionRegistrar { \
            ClassName##ReflectionRegistrar() { \
                ClassName##Reflection::registerReflection(); \
            } \
        }; \
        static ClassName##ReflectionRegistrar ClassName##_registrar; \
    }

#define REFLECT_PROPERTY(ClassName, PropertyName, PropertyType) \
    classInfo->addProperty(make_unique<Property>( \
        #PropertyName, \
        makeTypeInfo<PropertyType>(#PropertyType), \
        [](const void* obj) -> any { \
            return static_cast<const ClassName*>(obj)->PropertyName; \
        }, \
        [](void* obj, const any& value) { \
            static_cast<ClassName*>(obj)->PropertyName = any_cast<PropertyType>(value); \
        } \
    ));

#define REFLECT_METHOD_0(ClassName, MethodName, ReturnType) \
    classInfo->addMethod(make_unique<Method>( \
        #MethodName, \
        makeTypeInfo<ReturnType>(#ReturnType), \
        vector<TypeInfo>{}, \
        [](void* obj, const vector<any>&) -> any { \
            if constexpr (is_void_v<ReturnType>) { \
                static_cast<ClassName*>(obj)->MethodName(); \
                return any{}; \
            } else { \
                return static_cast<ClassName*>(obj)->MethodName(); \
            } \
        } \
    ));

#define REFLECT_METHOD_1(ClassName, MethodName, ReturnType, Param1Type) \
    classInfo->addMethod(make_unique<Method>( \
        #MethodName, \
        makeTypeInfo<ReturnType>(#ReturnType), \
        vector<TypeInfo>{makeTypeInfo<Param1Type>(#Param1Type)}, \
        [](void* obj, const vector<any>& params) -> any { \
            if constexpr (is_void_v<ReturnType>) { \
                static_cast<ClassName*>(obj)->MethodName(any_cast<Param1Type>(params[0])); \
                return any{}; \
            } else { \
                return static_cast<ClassName*>(obj)->MethodName(any_cast<Param1Type>(params[0])); \
            } \
        } \
    ));

class Person {
public:
    string name;
    int age;
    double height;
    
    Person() : name("Unknown"), age(0), height(0.0) {}
    Person(const string& n, int a, double h) : name(n), age(a), height(h) {}
    
    void introduce() {
        cout << "Hello, I'm " << name << ", " << age << " years old, " 
             << height << "m tall." << endl;
    }
    
    string getInfo() const {
        return name + " (" + to_string(age) + " years old)";
    }
    
    void setAge(int newAge) {
        age = newAge;
    }
    
    int calculateBirthYear() const {
        return 2024 - age;
    }
};

REFLECT_CLASS(Person)

void Person::PersonReflection::registerReflection() {
    auto classInfo = make_unique<ClassInfo>("Person");
    
    REFLECT_PROPERTY(Person, name, string)
    REFLECT_PROPERTY(Person, age, int)
    REFLECT_PROPERTY(Person, height, double)
    
    REFLECT_METHOD_0(Person, introduce, void)
    REFLECT_METHOD_0(Person, getInfo, string)
    REFLECT_METHOD_0(Person, calculateBirthYear, int)
    REFLECT_METHOD_1(Person, setAge, void, int)
    
    classInfo->setConstructor([]() -> unique_ptr<void, void(*)(void*)> {
        return unique_ptr<void, void(*)(void*)>(
            new Person(), 
            [](void* ptr) { delete static_cast<Person*>(ptr); }
        );
    });
    
    ReflectionRegistry::registerClass(move(classInfo));
}

class Calculator {
public:
    double value;
    
    Calculator() : value(0.0) {}
    
    double add(double x) {
        value += x;
        return value;
    }
    
    double multiply(double x) {
        value *= x;
        return value;
    }
    
    void reset() {
        value = 0.0;
    }
    
    double getValue() const {
        return value;
    }
};

REFLECT_CLASS(Calculator)

void Calculator::CalculatorReflection::registerReflection() {
    auto classInfo = make_unique<ClassInfo>("Calculator");
    
    REFLECT_PROPERTY(Calculator, value, double)
    
    REFLECT_METHOD_0(Calculator, reset, void)
    REFLECT_METHOD_0(Calculator, getValue, double)
    REFLECT_METHOD_1(Calculator, add, double, double)
    REFLECT_METHOD_1(Calculator, multiply, double, double)
    
    classInfo->setConstructor([]() -> unique_ptr<void, void(*)(void*)> {
        return unique_ptr<void, void(*)(void*)>(
            new Calculator(), 
            [](void* ptr) { delete static_cast<Calculator*>(ptr); }
        );
    });
    
    ReflectionRegistry::registerClass(move(classInfo));
}

void demonstratePropertyReflection() {
    cout << "\n=== Property Reflection Demo ===" << endl;
    
    const ClassInfo* personClass = ReflectionRegistry::getClass("Person");
    if (!personClass) {
        cout << "Person class not found!" << endl;
        return;
    }
    
    auto instance = personClass->createInstance();
    Person* person = static_cast<Person*>(instance.get());
    
    const Property* nameProp = personClass->getProperty("name");
    const Property* ageProp = personClass->getProperty("age");
    const Property* heightProp = personClass->getProperty("height");
    
    if (nameProp && ageProp && heightProp) {
        nameProp->setValue(person, string("John Doe"));
        ageProp->setValue(person, 30);
        heightProp->setValue(person, 1.75);
        
        cout << "Set properties via reflection:" << endl;
        cout << "Name: " << any_cast<string>(nameProp->getValue(person)) << endl;
        cout << "Age: " << any_cast<int>(ageProp->getValue(person)) << endl;
        cout << "Height: " << any_cast<double>(heightProp->getValue(person)) << endl;
    }
}

void demonstrateMethodReflection() {
    cout << "\n=== Method Reflection Demo ===" << endl;
    
    const ClassInfo* personClass = ReflectionRegistry::getClass("Person");
    if (!personClass) {
        cout << "Person class not found!" << endl;
        return;
    }
    
    auto instance = personClass->createInstance();
    Person* person = static_cast<Person*>(instance.get());
    
    person->name = "Alice Smith";
    person->age = 25;
    person->height = 1.65;
    
    const Method* introduceMethod = personClass->getMethod("introduce");
    const Method* getInfoMethod = personClass->getMethod("getInfo");
    const Method* setAgeMethod = personClass->getMethod("setAge");
    const Method* birthYearMethod = personClass->getMethod("calculateBirthYear");
    
    if (introduceMethod) {
        cout << "Calling introduce() via reflection:" << endl;
        introduceMethod->invoke(person, {});
    }
    
    if (getInfoMethod) {
        auto result = getInfoMethod->invoke(person, {});
        cout << "getInfo() result: " << any_cast<string>(result) << endl;
    }
    
    if (setAgeMethod) {
        cout << "Setting age to 26 via reflection..." << endl;
        setAgeMethod->invoke(person, {26});
        cout << "New age: " << person->age << endl;
    }
    
    if (birthYearMethod) {
        auto result = birthYearMethod->invoke(person, {});
        cout << "Birth year: " << any_cast<int>(result) << endl;
    }
}

void demonstrateCalculatorReflection() {
    cout << "\n=== Calculator Reflection Demo ===" << endl;
    
    const ClassInfo* calcClass = ReflectionRegistry::getClass("Calculator");
    if (!calcClass) {
        cout << "Calculator class not found!" << endl;
        return;
    }
    
    auto instance = calcClass->createInstance();
    Calculator* calc = static_cast<Calculator*>(instance.get());
    
    const Method* addMethod = calcClass->getMethod("add");
    const Method* multiplyMethod = calcClass->getMethod("multiply");
    const Method* resetMethod = calcClass->getMethod("reset");
    const Method* getValueMethod = calcClass->getMethod("getValue");
    
    if (addMethod && multiplyMethod && resetMethod && getValueMethod) {
        cout << "Performing calculations via reflection:" << endl;
        
        auto result1 = addMethod->invoke(calc, {vector<any>{10.0}});
        cout << "add(10.0) = " << any_cast<double>(result1) << endl;
        
        auto result2 = addMethod->invoke(calc, {vector<any>{5.0}});
        cout << "add(5.0) = " << any_cast<double>(result2) << endl;
        
        auto result3 = multiplyMethod->invoke(calc, {vector<any>{2.0}});
        cout << "multiply(2.0) = " << any_cast<double>(result3) << endl;
        
        auto currentValue = getValueMethod->invoke(calc, {});
        cout << "Current value: " << any_cast<double>(currentValue) << endl;
        
        resetMethod->invoke(calc, {});
        auto afterReset = getValueMethod->invoke(calc, {});
        cout << "After reset: " << any_cast<double>(afterReset) << endl;
    }
}

void demonstrateClassIntrospection() {
    cout << "\n=== Class Introspection Demo ===" << endl;
    
    auto classNames = ReflectionRegistry::getAllClassNames();
    cout << "Registered classes: ";
    for (const auto& name : classNames) {
        cout << name << " ";
    }
    cout << endl;
    
    for (const auto& className : classNames) {
        const ClassInfo* classInfo = ReflectionRegistry::getClass(className);
        if (classInfo) {
            cout << "\nClass: " << className << endl;
            
            auto properties = classInfo->getPropertyNames();
            cout << "  Properties: ";
            for (const auto& prop : properties) {
                cout << prop << " ";
            }
            cout << endl;
            
            auto methods = classInfo->getMethodNames();
            cout << "  Methods: ";
            for (const auto& method : methods) {
                cout << method << " ";
            }
            cout << endl;
        }
    }
}

void demonstrateGenericObjectManipulation() {
    cout << "\n=== Generic Object Manipulation ===" << endl;
    
    auto processObject = [](const string& className, const string& nameProperty, const string& nameValue) {
        const ClassInfo* classInfo = ReflectionRegistry::getClass(className);
        if (!classInfo) {
            cout << "Class " << className << " not found!" << endl;
            return;
        }
        
        auto instance = classInfo->createInstance();
        void* obj = instance.get();
        
        const Property* prop = classInfo->getProperty(nameProperty);
        if (prop) {
            prop->setValue(obj, nameValue);
            auto value = prop->getValue(obj);
            cout << "Set " << nameProperty << " to: " << any_cast<string>(value) << endl;
        }
        
        auto methods = classInfo->getMethodNames();
        for (const auto& methodName : methods) {
            if (methodName == "introduce" || methodName == "getInfo") {
                const Method* method = classInfo->getMethod(methodName);
                if (method && method->getParameterTypes().empty()) {
                    cout << "Calling " << methodName << "(): ";
                    auto result = method->invoke(obj, {});
                    if (methodName == "getInfo") {
                        cout << any_cast<string>(result) << endl;
                    } else {
                        cout << "(void method called)" << endl;
                    }
                }
            }
        }
    };
    
    processObject("Person", "name", "Reflected Person");
}

int main() {
    cout << "Advanced C++ Reflection System Demo" << endl;
    cout << "===================================" << endl;
    
    demonstratePropertyReflection();
    demonstrateMethodReflection();
    demonstrateCalculatorReflection();
    demonstrateClassIntrospection();
    demonstrateGenericObjectManipulation();
    
    cout << "\nReflection system demonstration completed!" << endl;
    return 0;
}