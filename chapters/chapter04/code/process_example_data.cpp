#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
// 过程式编程：数据处理简化版
int main() {
    int n;
    std::cout << "请输入数据个数：";
    std::cin >> n;
    std::vector<double> data(n);
    std::cout << "请输入 " << n << " 个数据（浮点数）：" << std::endl;
    for (auto& x : data) std::cin >> x;
    double sum = data.empty() ? 0.0 : std::accumulate(data.begin(), data.end(), 0.0);
    double avg = data.empty() ? 0.0 : sum / data.size();
    double min = data.empty() ? 0.0 : *std::min_element(data.begin(), data.end());
    double max = data.empty() ? 0.0 : *std::max_element(data.begin(), data.end());
    std::sort(data.begin(), data.end());
    double median = data.empty() ? 0.0 : data[data.size() / 2];
    double std = data.empty() ? 0.0 : std::sqrt(std::accumulate(data.begin(), data.end(), 0.0, [avg](double a, double b) {
        return a + (b - avg) * (b - avg);
    }) / data.size());
    double var = data.empty() ? 0.0 : std::accumulate(data.begin(), data.end(), 0.0, [avg](double a, double b) {
        return a + (b - avg) * (b - avg);
    }) / data.size();
    std::cout << "数据总和: " << sum << std::endl;
    std::cout << "数据个数: " << data.size() << std::endl;
    std::cout << "平均值: " << avg << std::endl;
    std::cout << "最小值: " << min << std::endl;
    std::cout << "最大值: " << max << std::endl;
    std::cout << "中位数: " << median << std::endl;
    std::cout << "标准差: " << std << std::endl;
    std::cout << "方差: " << var << std::endl;
    std::cin.get();
    return 0;
}
