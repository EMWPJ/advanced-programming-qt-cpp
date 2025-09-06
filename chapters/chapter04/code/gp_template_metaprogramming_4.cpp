#include <iostream>
#include <type_traits>
#include <string>
// 1. 移除指针类型
template<typename T> struct remove_pointer { using type = T; };
template<typename T> struct remove_pointer<T*> { using type = T; };
// 2. 添加const修饰符
template<typename T> struct add_const { using type = const T; };
// 3. 移除引用类型
template<typename T> struct remove_reference { using type = T; };
template<typename T> struct remove_reference<T&> { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };
int main() {
    std::cout << std::boolalpha;
    std::cout << "remove_pointer<int*>::type 是否为 int: "
              << std::is_same<remove_pointer<int*>::type, int>::value << std::endl;
    std::cout << "add_const<int>::type 是否为 const int: "
              << std::is_same<add_const<int>::type, const int>::value << std::endl;
    std::cout << "remove_reference<int&>::type 是否为 int: "
              << std::is_same<remove_reference<int&>::type, int>::value << std::endl;
    std::cout << "remove_reference<std::string&&>::type 是否为 std::string: "
              << std::is_same<remove_reference<std::string&&>::type, std::string>::value << std::endl;
    return 0;
}
