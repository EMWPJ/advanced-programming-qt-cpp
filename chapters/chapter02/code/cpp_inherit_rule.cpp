#include <iostream>
#include <windows.h>

// 基类，包含public、protected、private成员
class Base {
public:
    int publicVar;
    void publicFunc() { std::cout << "Base publicFunc" << std::endl; }
protected:
    int protectedVar;
    void protectedFunc() { std::cout << "Base protectedFunc" << std::endl; }
private:
    int privateVar;
    void privateFunc() { std::cout << "Base privateFunc" << std::endl; }
};

// 公有继承
class PublicDerived : public Base {
public:
    void test() {
        publicVar = 1;         // 可以访问
        protectedVar = 2;      // 可以访问
        // privateVar = 3;     // 无法访问
        publicFunc();          // 可以访问
        protectedFunc();       // 可以访问
        // privateFunc();      // 无法访问
        std::cout << "PublicDerived test" << std::endl;
    }
};

// 保护继承
class ProtectedDerived : protected Base {
public:
    void test() {
        publicVar = 1;         // 可以访问
        protectedVar = 2;      // 可以访问
        // privateVar = 3;     // 无法访问
        publicFunc();          // 可以访问
        protectedFunc();       // 可以访问
        // privateFunc();      // 无法访问
        std::cout << "ProtectedDerived test" << std::endl;
    }
};

// 私有继承
class PrivateDerived : private Base {
public:
    void test() {
        publicVar = 1;         // 可以访问
        protectedVar = 2;      // 可以访问
        // privateVar = 3;     // 无法访问
        publicFunc();          // 可以访问
        protectedFunc();       // 可以访问
        // privateFunc();      // 无法访问
        std::cout << "PrivateDerived test" << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);

    PublicDerived pub;
    pub.publicVar = 10;        // 可以访问
    pub.publicFunc();          // 可以访问
    // pub.protectedVar = 20;  // 无法访问
    // pub.protectedFunc();    // 无法访问
    pub.test();

    ProtectedDerived prot;
    // prot.publicVar = 10;    // 无法访问
    // prot.publicFunc();      // 无法访问
    prot.test();

    PrivateDerived priv;
    // priv.publicVar = 10;    // 无法访问
    // priv.publicFunc();      // 无法访问
    priv.test();

    std::cin.get();
    return 0;
}