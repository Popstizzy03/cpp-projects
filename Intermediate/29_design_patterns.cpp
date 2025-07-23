#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

class Singleton {
private:
    static unique_ptr<Singleton> instance;
    static mutex instanceMutex;
    string data;
    
    Singleton(const string& value) : data(value) {}
    
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
    static Singleton* getInstance(const string& value = "DefaultValue") {
        lock_guard<mutex> lock(instanceMutex);
        if (instance == nullptr) {
            instance = unique_ptr<Singleton>(new Singleton(value));
        }
        return instance.get();
    }
    
    string getData() const { return data; }
    void setData(const string& newData) { data = newData; }
};

unique_ptr<Singleton> Singleton::instance = nullptr;
mutex Singleton::instanceMutex;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& message) = 0;
};

class Subject {
private:
    vector<Observer*> observers;
    string state;
    
public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }
    
    void detach(Observer* observer) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void notify() {
        for (Observer* observer : observers) {
            observer->update(state);
        }
    }
    
    void setState(const string& newState) {
        state = newState;
        notify();
    }
    
    string getState() const { return state; }
};

class ConcreteObserver : public Observer {
private:
    string name;
    
public:
    ConcreteObserver(const string& n) : name(n) {}
    
    void update(const string& message) override {
        cout << "Observer " << name << " received: " << message << endl;
    }
};

class Product {
public:
    virtual ~Product() = default;
    virtual string operation() const = 0;
};

class ConcreteProductA : public Product {
public:
    string operation() const override {
        return "Result from ConcreteProductA";
    }
};

class ConcreteProductB : public Product {
public:
    string operation() const override {
        return "Result from ConcreteProductB";
    }
};

class Factory {
public:
    static unique_ptr<Product> createProduct(const string& type) {
        if (type == "A") {
            return make_unique<ConcreteProductA>();
        } else if (type == "B") {
            return make_unique<ConcreteProductB>();
        }
        return nullptr;
    }
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class Light {
private:
    bool isOn = false;
    
public:
    void turnOn() {
        isOn = true;
        cout << "Light is ON" << endl;
    }
    
    void turnOff() {
        isOn = false;
        cout << "Light is OFF" << endl;
    }
    
    bool getState() const { return isOn; }
};

class LightOnCommand : public Command {
private:
    Light* light;
    
public:
    LightOnCommand(Light* l) : light(l) {}
    
    void execute() override {
        light->turnOn();
    }
    
    void undo() override {
        light->turnOff();
    }
};

class LightOffCommand : public Command {
private:
    Light* light;
    
public:
    LightOffCommand(Light* l) : light(l) {}
    
    void execute() override {
        light->turnOff();
    }
    
    void undo() override {
        light->turnOn();
    }
};

class RemoteControl {
private:
    map<int, unique_ptr<Command>> commands;
    unique_ptr<Command> lastCommand;
    
public:
    void setCommand(int slot, unique_ptr<Command> command) {
        commands[slot] = move(command);
    }
    
    void pressButton(int slot) {
        if (commands.find(slot) != commands.end()) {
            commands[slot]->execute();
            lastCommand = move(commands[slot]);
        }
    }
    
    void pressUndo() {
        if (lastCommand) {
            lastCommand->undo();
        }
    }
};

int main() {
    cout << "Design Patterns Demo" << endl;
    cout << "===================" << endl;
    
    cout << "\n1. Singleton Pattern:" << endl;
    auto* singleton1 = Singleton::getInstance("First Instance");
    cout << "Singleton data: " << singleton1->getData() << endl;
    
    auto* singleton2 = Singleton::getInstance("Second Instance");
    cout << "Singleton data: " << singleton2->getData() << endl;
    cout << "Are they the same instance? " << (singleton1 == singleton2 ? "Yes" : "No") << endl;
    
    cout << "\n2. Observer Pattern:" << endl;
    Subject subject;
    ConcreteObserver observer1("Observer1");
    ConcreteObserver observer2("Observer2");
    ConcreteObserver observer3("Observer3");
    
    subject.attach(&observer1);
    subject.attach(&observer2);
    subject.attach(&observer3);
    
    subject.setState("State changed to A");
    subject.setState("State changed to B");
    
    subject.detach(&observer2);
    subject.setState("State changed to C (Observer2 detached)");
    
    cout << "\n3. Factory Pattern:" << endl;
    auto productA = Factory::createProduct("A");
    auto productB = Factory::createProduct("B");
    
    if (productA) {
        cout << "Product A: " << productA->operation() << endl;
    }
    if (productB) {
        cout << "Product B: " << productB->operation() << endl;
    }
    
    cout << "\n4. Command Pattern:" << endl;
    Light light;
    RemoteControl remote;
    
    auto lightOnCmd = make_unique<LightOnCommand>(&light);
    auto lightOffCmd = make_unique<LightOffCommand>(&light);
    
    remote.setCommand(1, move(lightOnCmd));
    remote.setCommand(2, move(lightOffCmd));
    
    cout << "Pressing button 1 (Light ON):" << endl;
    remote.pressButton(1);
    
    cout << "Pressing button 2 (Light OFF):" << endl;
    remote.pressButton(2);
    
    cout << "Pressing undo:" << endl;
    remote.pressUndo();
    
    return 0;
}