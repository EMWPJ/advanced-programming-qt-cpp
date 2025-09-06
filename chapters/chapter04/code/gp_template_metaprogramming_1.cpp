#include <iostream>
#include <string>

// 移除const修饰符
template<typename T> struct remove_const { using type = T; };
template<typename T> struct remove_const<const T> { using type = T; };

// 判断类型是否相同
template<typename T, typename U> struct is_same { static constexpr bool value = false; };
template<typename T> struct is_same<T, T> { static constexpr bool value = true; };

// 判断是否为指针
template<typename T> struct is_pointer { static constexpr bool value = false; };
template<typename T> struct is_pointer<T*> { static constexpr bool value = true; };

int main() {
    std::cout << std::boolalpha;
    std::cout << "remove_const<const int>::type 是否为 int: "
              << is_same<remove_const<const int>::type, int>::value << std::endl;
    std::cout << "is_same<int, int>: " << is_same<int, int>::value << std::endl;
    std::cout << "is_same<int, double>: " << is_same<int, double>::value << std::endl;
    std::cout << "is_pointer<int>: " << is_pointer<int>::value << std::endl;
    std::cout << "is_pointer<int*>: " << is_pointer<int*>::value << std::endl;
    std::cout << "is_pointer<std::string*>: " << is_pointer<std::string*>::value << std::endl;
    std::cin.get();
    return 0;
}
