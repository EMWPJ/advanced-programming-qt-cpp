#include <iostream>
#include <vector>
class Animal {
public:
    Animal(const std::string& name) : name(name) {}
    virtual void makeSound() const { std::cout << name << " 发出未知的叫声。\n"; }
    virtual ~Animal() {}
protected:
    std::string name;
};
class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}
    void makeSound() const override { std::cout << name << "：汪汪！\n"; }
};
class Cat : public Animal {
public:
    Cat(const std::string& name) : Animal(name) {}
    void makeSound() const override { std::cout << name << "：喵喵！\n"; }
};

int main() {
    Dog dog("小黑");
    Cat cat("小花");
    Animal generic("神秘动物");
    std::vector<Animal*> animals = {&dog, &cat, &generic};
    for (auto a : animals) a->makeSound();
    std::cin.get();
    return 0;
}
