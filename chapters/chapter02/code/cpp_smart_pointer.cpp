#include <iostream>
#include <memory>
#include <windows.h>

class Resource {
public:
    Resource() { std::cout << "资源创建" << std::endl; }
    ~Resource() { std::cout << "资源销毁" << std::endl; }
    void use() { std::cout << "资源使用" << std::endl; }
};

int main() {
    SetConsoleOutputCP(65001);

    // 智能指针示例
    std::unique_ptr<int> ptr1(new int(10));
    std::shared_ptr<int> ptr2 = std::make_shared<int>(20);
    std::weak_ptr<int> ptr3 = ptr2;

    std::cout << "ptr1: " << *ptr1 << std::endl;
    std::cout << "ptr2: " << *ptr2 << std::endl;

    if (auto shared = ptr3.lock()) {
        std::cout << "ptr3: " << *shared << std::endl;
    }
    std::cin.get();
    return 0;
}