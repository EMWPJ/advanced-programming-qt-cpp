#include <iostream>
#include <type_traits>
#include <string>
// SFINAE示例：只有当T支持operator<<时，才启用print方法
template<typename T, typename = void>
class Printer {
public:
    void print(const T&) { std::cout << "不支持输出该类型" << std::endl; }
};
// 偏特化：当T支持operator<<时，启用此版本
template<typename T>
class Printer<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> {
public:
    void print(const T& value) { std::cout << "输出: " << value << std::endl; }
};
struct NoOstreamType {};
int main() {
    Printer<int> intPrinter;
    intPrinter.print(123); // 支持输出
    Printer<std::string> strPrinter;
    strPrinter.print("Hello SFINAE"); // 支持输出
    Printer<NoOstreamType> noPrinter;
    noPrinter.print(NoOstreamType{}); // 不支持输出该类型
    std::cin.get();
    return 0;
}
