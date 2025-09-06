#include <iostream>
#include <string>
class Animal {
public:
    Animal(const std::string& name) : m_name(name) {}
    virtual ~Animal() {}
    virtual void speak() const = 0; // 纯虚函数
    std::string name() const { return m_name; }
protected:
    std::string m_name;
};
class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}

    void speak() const override {
        std::cout << "Dog " << m_name << " says: 汪汪!" << std::endl;
    }
};

class Cat : public Animal {
public:
    Cat(const std::string& name) : Animal(name) {}
    void speak() const override {
        std::cout << "Cat " << m_name << " says: 喵喵!" << std::endl;
    }
};

int main() {
    Animal* dog = new Dog("小黑");
    Animal* cat = new Cat("小花");
    dog->speak();
    cat->speak();
    delete dog;
    delete cat;
    std::cin.get();
    return 0;
}
