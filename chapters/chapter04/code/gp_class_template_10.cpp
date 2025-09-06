#include <iostream>
#include <string>
#include <tuple>
// 变长模板参数类，支持任意数量和类型的参数
template<typename... Args>
class VariadicPrinter {
public:
    // 构造函数，使用参数包初始化元组
    VariadicPrinter(const Args&... args) : m_values(args...) {}
    // 打印所有参数
    void print() const { printTuple(m_values); }
private:
    std::tuple<Args...> m_values; // 存储所有参数的元组
    // 递归打印元组中的每个元素
    template<std::size_t I = 0>
    void printTuple(const std::tuple<Args...>& t) const {
        if constexpr (I < sizeof...(Args)) { // 编译期判断是否还有元素
            std::cout << std::get<I>(t) << " "; // 打印第I个元素
            printTuple<I + 1>(t); // 递归打印下一个元素
        }}};
int main() {
    // 打印int、double、string类型的参数
    VariadicPrinter<int, double, std::string> printer(42, 3.14, "Hello");
    printer.print(); std::cout << std::endl;
    // 打印空参数（不会输出任何内容）
    VariadicPrinter<> emptyPrinter;
    emptyPrinter.print(); std::cout << std::endl;
    // 打印多个string和int类型参数
    VariadicPrinter<std::string, std::string, int> strPrinter("C++", "模板", 2023);
    strPrinter.print(); std::cout << std::endl;
    std::cin.get();
    return 0;
}
