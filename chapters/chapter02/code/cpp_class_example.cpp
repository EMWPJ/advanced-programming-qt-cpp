#include <iostream>
#include <string>
#include <windows.h>

class Student {
public:
    Student(std::string name, int age) : name(name), age(age) {}

    void display() {
        std::cout << "姓名: " << name << ", 年龄: " << age << std::endl;
    }

private:
    std::string name;
    int age;
};

int main() {
    SetConsoleOutputCP(65001);

    Student student("张三", 20);
    student.display();

    std::cin.get();
    return 0;
}