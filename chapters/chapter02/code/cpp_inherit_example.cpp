#include <iostream>
#include <windows.h>

class Animal {
public:
    void eat() {
        std::cout << "动物在吃" << std::endl;
    }
};

class Dog : public Animal {
public:
    void bark() {
        std::cout << "狗在叫" << std::endl;
    }
};

int main() {
    SetConsoleOutputCP(65001);

    Dog dog;
    dog.eat();
    dog.bark();

    std::cin.get();
    return 0;
}
