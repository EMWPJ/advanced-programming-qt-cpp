// 智能指针演示

#include <iostream>
#include <memory>
#include <string>
#include <windows.h>

class Resource {
public:
    Resource(const std::string& name) : name_(name) {
        std::cout << "资源 " << name_ << " 创建" << std::endl;
    }

    ~Resource() {
        std::cout << "资源 " << name_ << " 销毁" << std::endl;
    }

    void use() {
        std::cout << "使用资源 " << name_ << std::endl;
    }

private:
    std::string name_;
};

// unique_ptr - 独占所有权
void unique_ptr_demo() {
    std::cout << "\n=== unique_ptr Demo ===" << std::endl;

    // 创建unique_ptr
    std::unique_ptr<Resource> ptr1(new Resource("unique1"));
    auto ptr2 = std::make_unique<Resource>("unique2");

    // 使用资源
    ptr1->use();
    ptr2->use();

    // 转移所有权
    std::unique_ptr<Resource> ptr3 = std::move(ptr1);
    if (!ptr1) {
        std::cout << "ptr1 现在是空" << std::endl;
    }

    // 自动释放
} // ptr2和ptr3在这里自动销毁

// shared_ptr - 共享所有权
void shared_ptr_demo() {
    std::cout << "\n=== shared_ptr Demo ===" << std::endl;

    // 创建shared_ptr
    std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>("shared1");
    std::shared_ptr<Resource> ptr2 = ptr1; // 共享所有权

    std::cout << "Reference count: " << ptr1.use_count() << std::endl;

    // 使用资源
    ptr1->use();
    ptr2->use();

    // 释放一个引用
    ptr1.reset();
    std::cout << "ptr1.reset() 后，引用计数: " << ptr2.use_count() << std::endl;

    // 自动释放
} // ptr2在这里自动销毁

// weak_ptr - 弱引用
void weak_ptr_demo() {
    std::cout << "\n=== weak_ptr Demo ===" << std::endl;

    std::shared_ptr<Resource> shared = std::make_shared<Resource>("shared_for_weak");
    std::weak_ptr<Resource> weak = shared;

    std::cout << "共享引用计数: " << shared.use_count() << std::endl;
    std::cout << "弱引用计数: " << weak.use_count() << std::endl;

    // 通过weak_ptr访问资源
    if (auto locked = weak.lock()) {
        locked->use();
        std::cout << "成功锁定 weak_ptr" << std::endl;
    }

    // 释放shared_ptr
    shared.reset();

    // 尝试通过weak_ptr访问资源
    if (auto locked = weak.lock()) {
        locked->use();
    } else {
        std::cout << "弱_ptr 已过期" << std::endl;
    }
}

int main() {
    SetConsoleOutputCP(65001);
    unique_ptr_demo();
    shared_ptr_demo();
    weak_ptr_demo();

    std::cout << "\n所有演示完成" << std::endl;
    std::cin.get();
    return 0;
}