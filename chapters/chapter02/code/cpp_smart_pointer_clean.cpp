#include <iostream>
#include <memory>
#include <vector>
#include <windows.h>
class Resource {
public:
    Resource() { std::cout << "资源创建" << std::endl; }
    ~Resource() { std::cout << "资源销毁" << std::endl; }
    void use() { std::cout << "资源使用" << std::endl; }
};
int main() {
    SetConsoleOutputCP(65001);
    // unique_ptr - 独占所有权
    std::unique_ptr<Resource> unique = std::make_unique<Resource>();
    unique->use();
    // unique_ptr 不能被复制，只能被移动
    std::unique_ptr<Resource> unique2 = std::move(unique);
    // shared_ptr - 共享所有权
    std::shared_ptr<Resource> shared1 = std::make_shared<Resource>();
    {
        std::shared_ptr<Resource> shared2 = shared1;  // 引用计数 +1
        shared2->use();
        std::cout << "Reference count: " << shared1.use_count() << std::endl;
    }  // shared2 超出作用域，引用计数 -1
    std::cout << "Reference count: " << shared1.use_count() << std::endl;
    // weak_ptr - 弱引用
    std::weak_ptr<Resource> weak = shared1;
    std::cout << "weak_ptr reference count: " << weak.use_count() << std::endl;
    // 检查 weak_ptr 是否有效
    if (auto locked = weak.lock()) {  // 如果有效，使用资源
        locked->use();
    }
    std::cin.get();
    return 0;
}