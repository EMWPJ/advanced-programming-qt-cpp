#include <iostream>
#include <memory>
#include <windows.h>

using namespace std;

// 抽象产品：奶茶
class Tea {
public:
    virtual ~Tea() = default;
    virtual void drink() = 0;
};

// 抽象产品：配料
class Topping {
public:
    virtual ~Topping() = default;
    virtual void add() = 0;
};

// 具体产品：原味奶茶
class OriginalTea : public Tea {
public:
    void drink() override {
        cout << "喝一口原味奶茶" << endl;
    }
};
// 具体产品：甜味奶茶
class SweetTea : public Tea {
public:
    void drink() override {
        cout << "喝一口甜味奶茶" << endl;
    }
};
// 具体产品：咸味奶茶
class SaltyTea : public Tea {
public:
    void drink() override {
        cout << "喝一口咸味奶茶" << endl;
    }
};

// 具体产品：牛奶配料
class MilkTopping : public Topping {
public:
    void add() override {
        cout << "加入牛奶配料" << endl;
    }
};
// 具体产品：糖配料
class SugarTopping : public Topping {
public:
    void add() override {
        cout << "加入糖配料" << endl;
    }
};
// 具体产品：巧克力配料
class ChocolateTopping : public Topping {
public:
    void add() override {
        cout << "加入巧克力配料" << endl;
    }
};

// 抽象工厂：奶茶工厂
class TeaFactory {
public:
    virtual ~TeaFactory() = default;
    virtual unique_ptr<Tea> createTea() = 0;
    virtual unique_ptr<Topping> createTopping() = 0;
};
// 具体工厂：原味工厂
class OriginalFactory : public TeaFactory {
public:
    unique_ptr<Tea> createTea() override {
        return make_unique<OriginalTea>();
    }
    unique_ptr<Topping> createTopping() override {
        return make_unique<MilkTopping>();
    }
};

// 具体工厂：甜味工厂
class SweetFactory : public TeaFactory {
public:
    unique_ptr<Tea> createTea() override {
        return make_unique<SweetTea>();
    }
    unique_ptr<Topping> createTopping() override {
        return make_unique<SugarTopping>();
    }
};
// 具体工厂：巧克力味工厂
class ChocolateFactory : public TeaFactory {
public:
    unique_ptr<Tea> createTea() override {
        return make_unique<ChocolateTea>();
    }
    unique_ptr<Topping> createTopping() override {
        return make_unique<ChocolateTopping>();
    }
};

// 客户端代码
void clientCode(unique_ptr<TeaFactory> factory) {
    auto tea = factory->createTea();
    auto topping = factory->createTopping();
    cout << "制作奶茶..." << endl;
    topping->add();
    tea->drink();
}
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== 抽象工厂模式演示（奶茶店） ===" << endl;
    cout << "\n使用原味工厂:" << endl;
    clientCode(make_unique<OriginalFactory>());
    cout << "\n使用甜味工厂:" << endl;
    clientCode(make_unique<SweetFactory>());
    cout << "\n使用巧克力味工厂:" << endl;
    clientCode(make_unique<ChocolateFactory>());
    cin.get();
    return 0;
}