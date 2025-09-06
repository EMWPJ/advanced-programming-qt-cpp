#include <iostream>
#include <vector>
// 过程式编程：算法实现示例
void inputData(std::vector<int>& data, int n) {
    std::cout << "请输入 " << n << " 个整数：" << std::endl;
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        data.push_back(x);
    }
}
void bubbleSort(std::vector<int>& data) {
    size_t n = data.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - 1 - i; ++j) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
    }
}
void printData(const std::vector<int>& data) {
    std::cout << "排序后的结果为：" << std::endl;
    for (int x : data) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}
int main() {
    int n;
    std::cout << "请输入数据个数：";
    std::cin >> n;
    std::vector<int> data;
    inputData(data, n);
    bubbleSort(data);
    printData(data);
    std::cin.get();
    return 0;
}
