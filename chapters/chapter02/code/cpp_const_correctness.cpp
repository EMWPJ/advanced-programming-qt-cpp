#include <iostream>
#include <string>
#include <windows.h>

class StringProcessor {
private:
    std::string data;

public:
    StringProcessor(const std::string& str) : data(str) {}

    // const成员函数 - 不修改对象状态
    std::string getData() const {
        return data;
    }

    // const成员函数 - 只读访问
    size_t length() const {
        return data.length();
    }

    // 非const成员函数 - 可以修改对象状态
    void append(const std::string& str) {
        data += str;
    }

    // const引用参数 - 避免复制
    bool contains(const std::string& substr) const {
        return data.find(substr) != std::string::npos;
    }
};

void processString(const StringProcessor& processor) {
    // 只能调用const成员函数
    std::cout << "Length: " << processor.length() << std::endl;
    std::cout << "Contains 'hello': " << processor.contains("hello") << std::endl;
}

int main() {
    SetConsoleOutputCP(65001);

    const int MAX_SIZE = 100;
    const int* ptr = &MAX_SIZE;

    std::cout << "MAX_SIZE = " << *ptr << std::endl;

    std::cin.get();
    return 0;
}