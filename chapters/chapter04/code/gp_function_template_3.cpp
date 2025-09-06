#include <iostream>
#include <type_traits>

// 通用模板
template<typename T1, typename T2>
auto TemplateAdd(const T1& a, const T2& b) -> decltype(a + b) {
    return a + b;
}

// 针对bool类型的重载
bool TemplateAddBool(const bool& a, const bool& b) {
    return a || b;
}

int main() {
    bool a = true, b = false;
    std::cout << std::boolalpha;
    std::cout << "TemplateAdd(a, b) = " << TemplateAddBool(a, b) << std::endl;

    int x = 5, y = 7;
    std::cout << "TemplateAdd(x, y) = " << TemplateAdd(x, y) << std::endl;
    std::cin.get();
    return 0;
}
