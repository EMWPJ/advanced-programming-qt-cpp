#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <windows.h>

class StringWrapper {
private:
    std::string data;

public:
    // 构造函数
    StringWrapper(const std::string& str) : data(str) {
        std::cout << "拷贝构造函数" << std::endl;
    }

    // 移动构造函数
    StringWrapper(StringWrapper&& other) noexcept
        : data(std::move(other.data)) {
        std::cout << "移动构造函数" << std::endl;
    }

    // 拷贝赋值运算符
    StringWrapper& operator=(const StringWrapper& other) {
        if (this != &other) {
            data = other.data;
            std::cout << "拷贝赋值运算符" << std::endl;
        }
        return *this;
    }

    // 移动赋值运算符
    StringWrapper& operator=(StringWrapper&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            std::cout << "移动赋值运算符" << std::endl;
        }
        return *this;
    }

    void print() const {
        std::cout << "Data: " << data << std::endl;
    }
};

StringWrapper createString() {
    return StringWrapper("Temporary string");
}

class Resource {
public:
    Resource() { std::cout << "Resource acquired" << std::endl; }
    ~Resource() { std::cout << "Resource destroyed" << std::endl; }
};

int main() {
    SetConsoleOutputCP(65001);

    std::vector<Resource> resources;
    resources.push_back(Resource()); // 移动语义

    std::cin.get();
    return 0;
}