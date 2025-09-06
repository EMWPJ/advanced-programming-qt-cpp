#include <iostream>
#include <string>
#include <cstring>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);

    // C风格字符串
    char cString[] = "Hello, C++";
    std::cout << "C风格字符串: " << cString << std::endl;
    std::cout << "字符串长度: " << strlen(cString) << std::endl;

    // std::string
    std::string cppString = "Hello, Modern C++";
    std::cout << "C++字符串: " << cppString << std::endl;
    std::cout << "字符串长度: " << cppString.length() << std::endl;

    // 字符串连接
    std::string str1 = "Hello";
    std::string str2 = "World";
    std::string result = str1 + ", " + str2 + "!";
    std::cout << "连接结果: " << result << std::endl;

    // 字符串查找
    std::string text = "C++ is a powerful language";
    size_t pos = text.find("powerful");
    if (pos != std::string::npos) {
        std::cout << "找到'powerful'在位置: " << pos << std::endl;
    }

    // 字符串替换
    std::string message = "Hello World";
    message.replace(6, 5, "C++");
    std::cout << "替换后: " << message << std::endl;

    // 字符串比较
    std::string s1 = "apple";
    std::string s2 = "banana";
    if (s1 < s2) {
        std::cout << s1 << " 在字典序中小于 " << s2 << std::endl;
    }

    std::cin.get();
    return 0;
}