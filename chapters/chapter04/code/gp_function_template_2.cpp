#include <iostream>

#include <type_traits>

template<typename T1, typename T2>
auto TemplateAdd(const T1& a, const T2& b) -> decltype(a + b) {
    return a + b;
}

float NormalAdd(const int& a, const float& b) {
    return a + b;
}

float NormalAdd(const float& a, const int& b) {
    return a + b;
}

double NormalAdd(const double& a, const int& b) {
    return a + b;
}

double NormalAdd(const int& a, const double& b) {
    return a + b;
}

double NormalAdd(const double& a, const float& b) {
    return a + b;
}

double NormalAdd(const float& a, const double& b) {
    return a + b;
}

int NormalAdd(const int& a, const int& b) {
    return a + b;
}

float NormalAdd(const float& a, const float& b) {
    return a + b;
}

double NormalAdd(const double& a, const double& b) {
    return a + b;
}

int main() {
    int a = 10;
    float c = 10.1;
    double e = 10.3;
    std::cout << "TemplateAdd(a, c) = " << TemplateAdd(a, c) << std::endl;
    std::cout << "NormalAdd(a, c) = " << NormalAdd(a, c) << std::endl;
    std::cout << "TemplateAdd(a, e) = " << TemplateAdd(a, e) << std::endl;
    std::cout << "NormalAdd(a, e) = " << NormalAdd(a, e) << std::endl;
    std::cout << "TemplateAdd(c, e) = " << TemplateAdd(c, e) << std::endl;
    std::cout << "NormalAdd(c, e) = " << NormalAdd(c, e) << std::endl;
    std::cin.get();
    return 0;
}
