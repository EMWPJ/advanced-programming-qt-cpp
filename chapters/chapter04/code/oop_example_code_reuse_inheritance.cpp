#include <iostream>
#include <string>
class Animal {
public:
    Animal(const std::string& name) : name(name) {}
    void eat() const {
        std::cout << name << " 正在吃东西。" << std::endl;
    }
    void sleep() const {
        std::cout << name << " 正在睡觉。" << std::endl;
    }
protected:
    std::string name;
};
class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}
    void bark() const {
        std::cout << name << "：汪汪！" << std::endl;
    }
};
class Cat : public Animal {
public:
    Cat(const std::string& name) : Animal(name) {}
    void meow() const {
        std::cout << name << "：喵喵！" << std::endl;
    }
};
int main() {
    Dog dog("小黑");
    Cat cat("小花");
    dog.eat();
    dog.bark();
    dog.sleep();
    cat.eat();
    cat.meow();
    cat.sleep();
    std::cin.get();
    return 0;
}
