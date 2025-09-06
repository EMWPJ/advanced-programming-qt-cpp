#include <iostream>
#include <string>

// 多参数类模板定义
template<typename T1, typename T2>
class MyPair {
public:
    MyPair(const T1& first, const T2& second)
        : m_first(first), m_second(second) {}

    T1 getFirst() const { return m_first; }
    T2 getSecond() const { return m_second; }

    void print() const {
        std::cout << "First: " << m_first << ", Second: " << m_second << std::endl;
    }

private:
    T1 m_first;
    T2 m_second;
};

int main() {
    MyPair<int, double> pair1(10, 3.14);
    pair1.print();

    MyPair<std::string, int> pair2("Age", 30);
    pair2.print();

    MyPair<std::string, std::string> pair3("Hello", "World");
    pair3.print();
    std::cin.get();
    return 0;
}
