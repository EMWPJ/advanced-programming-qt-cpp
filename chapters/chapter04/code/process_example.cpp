#include <iostream>
#include <vector>
#include <algorithm>
// 过程式编程：数据处理示例
// 读取一组整数，排序后输出偶数
void inputData(std::vector<int>& data, int n) {
    std::cout << "请输入 " << n << " 个整数：" << std::endl;
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        data.push_back(x);
    }
}
void sortData(std::vector<int>& data) {
    std::sort(data.begin(), data.end());
}
void printEven(const std::vector<int>& data) {
    std::cout << "排序后的偶数有：" << std::endl;
    for (int x : data) {
        if (x % 2 == 0) { std::cout << x << " "; }
    }
    std::cout << std::endl;
}
int main() {
    int n;
    std::cout << "请输入数据个数：";
    std::cin >> n;
    std::vector<int> data;
    inputData(data, n);
    sortData(data);
    printEven(data);
    std::cin.get();
    return 0;
}
