#include <iostream>
#include <windows.h>

class Shape {
public:
    virtual void draw() { std::cout << "绘制形状" << std::endl; }
    virtual ~Shape() = default; // 添加虚析构函数，防止内存泄漏
};

class Circle : public Shape {
public:
    void draw() override { std::cout << "绘制圆形" << std::endl; }
};

class Triangle : public Shape {
public:
    void draw() override { std::cout << "绘制三角形" << std::endl; }
};

int main() {
    SetConsoleOutputCP(65001);

    Shape* shape1 = new Circle();
    shape1->draw();

    Shape* shape2 = new Triangle();
    shape2->draw();

    delete shape1;
    delete shape2;

    std::cin.get();
    return 0;
}
