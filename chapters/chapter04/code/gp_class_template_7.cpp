#include <iostream>
#include <string>

// 类模板，包含成员模板方法
template<typename T>
class Adder {
public:
    Adder(const T& a, const T& b) : m_a(a), m_b(b) {}
    T sum() const { return m_a + m_b; }
    void print() const { std::cout << "Sum: " << sum() << std::endl; }
    // 成员模板：可以将任意类型U的Adder与当前对象的sum结果相加
    template<typename U>
    auto addWith(const Adder<U>& other) const { return sum() + other.sum(); }
private:
    T m_a, m_b;
};

int main() {
    Adder<int> intAdder(10, 20);
    intAdder.print();
    Adder<double> doubleAdder(3.14, 2.71);
    doubleAdder.print();
    Adder<std::string> stringAdder("Hello, ", "World!");
    stringAdder.print();
    // 使用成员模板，将不同类型Adder的sum结果相加
    auto mixedSum = intAdder.addWith(doubleAdder); // int + double
    std::cout << "intAdder + doubleAdder = " << mixedSum << std::endl;
    // 字符串与intAdder的sum结果相加（需要转换为字符串）
    // 这里演示成员模板的灵活性
    // std::string + int 需要先转换
    std::string strSum = stringAdder.sum() + std::to_string(intAdder.sum());
    std::cout << "stringAdder + intAdder = " << strSum << std::endl;
    std::cin.get();
    return 0;
}
