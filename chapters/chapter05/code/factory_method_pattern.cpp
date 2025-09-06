#include <iostream>
#include <memory>
#include <windows.h>

using namespace std;

class Beverage { // 抽象产品：饮料
public:
    virtual ~Beverage() = default;
    virtual void use() = 0;
};

class ConcreteCola : public Beverage { // 具体产品：可乐
public:
    void use() override { cout << "这是可乐" << endl; }
};

class ConcreteJuice : public Beverage { // 具体产品：果汁
public:
    void use() override { cout << "这是果汁" << endl; }
};

class ConcreteTea : public Beverage { // 具体产品：茶
public:
    void use() override { cout << "这是茶" << endl; }
};

class Creator { // 抽象工厂：饮料工厂
public:
    virtual ~Creator() = default;
    virtual unique_ptr<Beverage> createProduct() = 0;
    void someOperation() { auto product = createProduct(); }
};

class ConcreteCreatorA : public Creator { // 具体工厂：生产可乐的工厂
public:
    unique_ptr<Beverage> createProduct() override { return make_unique<ConcreteCola>(); }
};

class ConcreteCreatorB : public Creator { // 具体工厂：生产果汁的工厂
public:
    unique_ptr<Beverage> createProduct() override { return make_unique<ConcreteJuice>(); }
};

class ConcreteCreatorC : public Creator { // 具体工厂：生产茶的工厂
public:
    unique_ptr<Beverage> createProduct() override { return make_unique<ConcreteTea>(); }
};

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    cout << "=== 工厂方法模式演示 ===" << endl;

    cout << "\n1. 使用可乐工厂:" << endl;
    unique_ptr<Creator> colaFactory = make_unique<ConcreteCreatorA>();
    colaFactory->someOperation();

    cout << "\n2. 使用果汁工厂:" << endl;
    unique_ptr<Creator> juiceFactory = make_unique<ConcreteCreatorB>();
    juiceFactory->someOperation();

    cout << "\n3. 使用茶工厂:" << endl;
    unique_ptr<Creator> teaFactory = make_unique<ConcreteCreatorC>();
    teaFactory->someOperation();

    cin.get();
    return 0;
}
