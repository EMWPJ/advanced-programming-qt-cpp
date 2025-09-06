#include <iostream>
#include <variant>
#include <string>
// 定义一个简单的代数数据类型（ADT）：Shape
struct Circle { double radius; };
struct Rectangle { double width, height; };
// 使用std::variant表示Shape的“和类型”
using Shape = std::variant<Circle, Rectangle>;
// C++17及以上：实现模式匹配工具
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
double area(const Shape& shape) {
    // 使用std::visit进行“模式匹配”
    return std::visit(overloaded{
        [](const Circle& c) { return 3.14159 * c.radius * c.radius; },
        [](const Rectangle& r) { return r.width * r.height; }
    }, shape);
}
std::string shape_name(const Shape& shape) {
    return std::visit(overloaded{
        [](const Circle&) { return std::string("Circle"); },
        [](const Rectangle&) { return std::string("Rectangle"); }
    }, shape);
}
int main() {
    Shape s1 = Circle{2.0};
    Shape s2 = Rectangle{3.0, 4.0};
    for (const auto& s : {s1, s2}) {
        std::cout << shape_name(s) << " 的面积: " << area(s) << std::endl;
    }
    std::cin.get();
    return 0;
}
