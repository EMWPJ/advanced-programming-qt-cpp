#include <iostream>

template<typename T>
T TemplateAdd(const T& a, const T& b) {
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
    int a = 10, b = 20;
    std::cout << "TemplateAdd(a, b) = " << TemplateAdd(a, b) << std::endl;
    std::cout << "NormalAdd(a, b) = " << NormalAdd(a, b) << std::endl;
    float x = 3.14, y = 2.71;
    std::cout << "TemplateAdd(x, y) = " << TemplateAdd(x, y) << std::endl;
    std::cout << "NormalAdd(x, y) = " << NormalAdd(x, y) << std::endl;
    double z = 3.14, w = 2.71;
    std::cout << "TemplateAdd(z, w) = " << TemplateAdd(z, w) << std::endl;
    std::cout << "NormalAdd(z, w) = " << NormalAdd(z, w) << std::endl;
    std::cin.get();
    return 0;
}
