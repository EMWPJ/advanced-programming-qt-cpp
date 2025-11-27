#include <iostream>
#include <vector>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/poisson_distribution.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <windows.h>

using namespace std;
using namespace boost::random;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost随机数生成示例 ===" << endl;

    // 使用random_device作为种子
    random_device rd;
    mt19937 gen(rd());

    cout << "随机数生成器示例:" << endl;

    // 均匀分布 [0, 1)
    uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    cout << "均匀分布 [0, 1):" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << uniform_dist(gen) << " ";
    }
    cout << endl << endl;

    // 正态分布
    normal_distribution<double> normal_dist(0.0, 1.0); // 标准正态分布
    cout << "标准正态分布 N(0, 1):" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << normal_dist(gen) << " ";
    }
    cout << endl << endl;

    // 泊松分布
    poisson_distribution<int> poisson_dist(3.0); // λ = 3.0
    cout << "泊松分布 (λ = 3.0):" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << poisson_dist(gen) << " ";
    }
    cout << endl << endl;

    // 指数分布
    exponential_distribution<double> exp_dist(0.5); // λ = 0.5
    cout << "指数分布 (λ = 0.5):" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << exp_dist(gen) << " ";
    }
    cout << endl << endl;

    // 离散均匀分布
    uniform_int_distribution<int> int_dist(1, 6); // 骰子
    cout << "离散均匀分布 [1, 6] (模拟骰子):" << endl;
    for (int i = 0; i < 20; ++i) {
        cout << int_dist(gen) << " ";
        if ((i + 1) % 10 == 0) cout << endl;
    }
    cout << endl;

    // 生成大样本进行统计检验
    const int sample_size = 10000;
    vector<double> uniform_samples;
    vector<double> normal_samples;

    uniform_real_distribution<double> uniform_test(0.0, 1.0);
    normal_distribution<double> normal_test(5.0, 2.0);

    for (int i = 0; i < sample_size; ++i) {
        uniform_samples.push_back(uniform_test(gen));
        normal_samples.push_back(normal_test(gen));
    }

    // 计算样本统计量
    double uniform_mean = 0.0, normal_mean = 0.0;
    double uniform_var = 0.0, normal_var = 0.0;

    for (double val : uniform_samples) {
        uniform_mean += val;
    }
    uniform_mean /= sample_size;

    for (double val : uniform_samples) {
        uniform_var += (val - uniform_mean) * (val - uniform_mean);
    }
    uniform_var /= (sample_size - 1);

    for (double val : normal_samples) {
        normal_mean += val;
    }
    normal_mean /= sample_size;

    for (double val : normal_samples) {
        normal_var += (val - normal_mean) * (val - normal_mean);
    }
    normal_var /= (sample_size - 1);

    cout << "大样本统计检验:" << endl;
    cout << "均匀分布 [0,1) 样本 (" << sample_size << "个):" << endl;
    cout << "  样本均值: " << uniform_mean << " (理论值: 0.5)" << endl;
    cout << "  样本方差: " << uniform_var << " (理论值: 1/12 ≈ 0.0833)" << endl;

    cout << "正态分布 N(5,4) 样本 (" << sample_size << "个):" << endl;
    cout << "  样本均值: " << normal_mean << " (理论值: 5.0)" << endl;
    cout << "  样本方差: " << normal_var << " (理论值: 4.0)" << endl;

    // 随机种子示例
    cout << "\n使用固定种子保证可重现性:" << endl;
    mt19937 gen_fixed(42); // 固定种子
    uniform_real_distribution<double> fixed_dist(0.0, 1.0);

    cout << "固定种子生成序列:" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << fixed_dist(gen_fixed) << " ";
    }
    cout << endl;

    // 重新创建相同的生成器
    mt19937 gen_fixed2(42);
    uniform_real_distribution<double> fixed_dist2(0.0, 1.0);

    cout << "重新创建的相同序列:" << endl;
    for (int i = 0; i < 5; ++i) {
        cout << fixed_dist2(gen_fixed2) << " ";
    }
    cout << endl;

    cin.get();
    return 0;
}
