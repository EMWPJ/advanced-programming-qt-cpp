#include <iostream>

// 类模板定义
template<typename T>
class MyPair {
public:
    MyPair(const T& first, const T& second)
        : m_first(first), m_second(second) {}

    T getFirst() const { return m_first; }
    T getSecond() const { return m_second; }

    void print() const {
        std::cout << "First: " << m_first << ", Second: " << m_second << std::endl;
    }

private:
    T m_first;
    T m_second;
};

int main() {
    MyPair<int> intPair(10, 20);
    intPair.print();

    MyPair<double> doublePair(3.14, 2.71);
    doublePair.print();

    MyPair<std::string> stringPair("Hello", "World");
    stringPair.print();

    std::cin.get();
    return 0;
}
