#include <iostream>

// 非类型模板参数：指针
template<typename T, T* ptr>
class PointerWrapper {
public:
    void print() const {
        std::cout << "Pointer value: " << *ptr << std::endl;
    }
};

// 非类型模板参数：引用
template<typename T, T& ref>
class ReferenceWrapper {
public:
    void print() const {
        std::cout << "Reference value: " << ref << std::endl;
    }
};

int globalInt = 42;
double globalDouble = 3.14;
int main() {
    // 指针作为非类型模板参数
    PointerWrapper<int, &globalInt> intPtrWrapper;
    intPtrWrapper.print();

    PointerWrapper<double, &globalDouble> doublePtrWrapper;
    doublePtrWrapper.print();

    // 引用作为非类型模板参数
    ReferenceWrapper<int, globalInt> intRefWrapper;
    intRefWrapper.print();

    ReferenceWrapper<double, globalDouble> doubleRefWrapper;
    doubleRefWrapper.print();

    std::cin.get();
    return 0;
}
