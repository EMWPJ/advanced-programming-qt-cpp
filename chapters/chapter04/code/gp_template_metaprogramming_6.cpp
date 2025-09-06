#include <iostream>
#include <type_traits>

// 1. 编译时多态：根据类型选择不同实现（类型分支）
template<typename T>
struct TypePrinter {
    static void print() { std::cout << "未知类型" << std::endl; }
};

template<>
struct TypePrinter<int> {
    static void print() { std::cout << "类型为 int" << std::endl; }
};

template<>
struct TypePrinter<double> {
    static void print() { std::cout << "类型为 double" << std::endl; }
};

template<>
struct TypePrinter<const char*> {
    static void print() { std::cout << "类型为 const char*" << std::endl; }
};

// 2. 利用SFINAE实现编译时多态（判断是否为指针类型）
template<typename T>
typename std::enable_if<std::is_pointer<T>::value, void>::type
pointer_info(T) { std::cout << "是指针类型" << std::endl; }

template<typename T>
typename std::enable_if<!std::is_pointer<T>::value, void>::type
pointer_info(T) { std::cout << "不是指针类型" << std::endl; }

int main() {
    // 类型分支演示
    TypePrinter<int>::print();
    TypePrinter<double>::print();
    TypePrinter<const char*>::print();
    TypePrinter<float>::print();
    // SFINAE演示
    int a = 10;
    int* p = &a;
    pointer_info(a);   // 不是指针类型
    pointer_info(p);   // 是指针类型
    double d = 3.14;
    double* dp = &d;
    pointer_info(d);   // 不是指针类型
    pointer_info(dp);  // 是指针类型
    std::cin.get();
    return 0;
}
