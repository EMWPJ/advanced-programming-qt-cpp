#include <iostream>

// 1. 编译期循环：计算数组所有元素之和（递归模板）
template<int N>
struct Sum {
    template<typename T> static T sum(const T* arr) { return arr[0] + Sum<N - 1>::sum(arr + 1); }
};
template<>
struct Sum<0> { template<typename T> static T sum(const T*) { return T{}; } };

// 2. 编译期循环：打印数组所有元素（递归模板）
template<int N>
struct PrintArray {
    template<typename T> static void print(const T* arr) { std::cout << arr[0] << " "; PrintArray<N - 1>::print(arr + 1); }
};
template<>
struct PrintArray<0> { template<typename T> static void print(const T*) {} };

int main() {
    int arr[5] = {1, 2, 3, 4, 5};
    constexpr int N = sizeof(arr) / sizeof(arr[0]);
    std::cout << "数组元素: ";
    PrintArray<N>::print(arr);
    std::cout << "\n数组元素之和: " << Sum<N>::sum(arr) << std::endl;

    double darr[4] = {1.1, 2.2, 3.3, 4.4};
    constexpr int DN = sizeof(darr) / sizeof(darr[0]);
    std::cout << "double数组元素: ";
    PrintArray<DN>::print(darr);
    std::cout << "\ndouble数组元素之和: " << Sum<DN>::sum(darr) << std::endl;

    std::cin.get();
    return 0;
}
