#include <iostream>
#include <windows.h>

class Base1 {
public:
    void show1() { std::cout << "Base1 show" << std::endl; }
};

class Base2 {
public:
    void show2() { std::cout << "Base2 show" << std::endl; }
};

class Derived : public Base1, public Base2 {
public:
    void show() { std::cout << "Derived show" << std::endl; }
};

int main() {
    SetConsoleOutputCP(65001);

    Derived d;
    d.show1();
    d.show2();
    d.show();

    std::cin.get();
    return 0;
}
