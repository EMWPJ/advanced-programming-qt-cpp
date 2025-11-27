#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <Eigen/Dense>
#include <cmath>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// Eigen Array类型详细示例：元素级运算

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen Array类型详细示例 ===" << endl;

    // ========== 1. Array创建和基本操作 ==========
    cout << "\n--- 1. Array创建和基本操作 ---" << endl;

    ArrayXd arr1(5);
    arr1 << 1, 2, 3, 4, 5;

    ArrayXd arr2(5);
    arr2 << 2, 3, 4, 5, 6;

    cout << "arr1:\n" << arr1 << endl;
    cout << "arr2:\n" << arr2 << endl;

    // 元素级运算
    cout << "arr1 + arr2 (元素级加法):\n" << arr1 + arr2 << endl;
    cout << "arr1 * arr2 (元素级乘法):\n" << arr1 * arr2 << endl;
    cout << "arr2 / arr1 (元素级除法):\n" << arr2 / arr1 << endl;
    cout << "arr1 - arr2 (元素级减法):\n" << arr1 - arr2 << endl;

    // ========== 2. Array与Matrix转换 ==========
    cout << "\n--- 2. Array与Matrix转换 ---" << endl;

    MatrixXd mat(3, 3);
    mat << 1, 2, 3,
           4, 5, 6,
           7, 8, 9;

    cout << "Matrix:\n" << mat << endl;

    // Matrix转Array
    ArrayXXd arr = mat.array();
    cout << "转换为Array:\n" << arr << endl;

    // Array转Matrix
    MatrixXd mat2 = arr.matrix();
    cout << "转换回Matrix:\n" << mat2 << endl;

    // ========== 3. Array元素级函数 ==========
    cout << "\n--- 3. Array元素级函数 ---" << endl;

    ArrayXd arr3(5);
    arr3 << 0.5, 1.0, 2.0, 3.0, 4.0;

    cout << "原始数组:\n" << arr3 << endl;
    cout << "abs():\n" << arr3.abs() << endl;
    cout << "sqrt():\n" << arr3.sqrt() << endl;
    cout << "exp():\n" << arr3.exp() << endl;
    cout << "log():\n" << arr3.log() << endl;
    cout << "sin():\n" << arr3.sin() << endl;
    cout << "cos():\n" << arr3.cos() << endl;
    cout << "pow(2):\n" << arr3.pow(2) << endl;

    // ========== 4. Array条件操作 ==========
    cout << "\n--- 4. Array条件操作 ---" << endl;

    ArrayXd arr4(5);
    arr4 << -2, -1, 0, 1, 2;

    cout << "原始数组:\n" << arr4 << endl;

    // 条件选择
    ArrayXd arr5(5);
    arr5 << 10, 20, 30, 40, 50;

    // 如果arr4 > 0，选择arr4，否则选择arr5
    ArrayXd result = (arr4 > 0).select(arr4, arr5);
    cout << "条件选择 (arr4>0选arr4，否则选arr5):\n" << result << endl;

    // 条件赋值
    ArrayXd arr6 = arr4;
    arr6 = (arr6 < 0).select(-arr6, arr6);  // 取绝对值
    cout << "条件赋值（取绝对值）:\n" << arr6 << endl;

    // ========== 5. Array统计操作 ==========
    cout << "\n--- 5. Array统计操作 ---" << endl;

    ArrayXd arr7(6);
    arr7 << 1, 5, 3, 9, 2, 7;

    cout << "数组:\n" << arr7 << endl;
    cout << "最小值: " << arr7.minCoeff() << endl;
    cout << "最大值: " << arr7.maxCoeff() << endl;
    cout << "和: " << arr7.sum() << endl;
    cout << "均值: " << arr7.mean() << endl;
    cout << "乘积: " << arr7.prod() << endl;

    // ========== 6. Array与标量运算 ==========
    cout << "\n--- 6. Array与标量运算 ---" << endl;

    ArrayXd arr8(4);
    arr8 << 1, 2, 3, 4;

    cout << "原始数组:\n" << arr8 << endl;
    cout << "arr8 + 10:\n" << arr8 + 10 << endl;
    cout << "arr8 * 2:\n" << arr8 * 2 << endl;
    cout << "arr8 / 2:\n" << arr8 / 2 << endl;
    cout << "arr8 - 1:\n" << arr8 - 1 << endl;

    // ========== 7. Array比较操作 ==========
    cout << "\n--- 7. Array比较操作 ---" << endl;

    ArrayXd arr9(5);
    arr9 << 1, 2, 3, 4, 5;

    ArrayXd arr10(5);
    arr10 << 2, 2, 3, 5, 4;

    cout << "arr9:\n" << arr9 << endl;
    cout << "arr10:\n" << arr10 << endl;
    cout << "arr9 == arr10:\n" << (arr9 == arr10) << endl;
    cout << "arr9 != arr10:\n" << (arr9 != arr10) << endl;
    cout << "arr9 > arr10:\n" << (arr9 > arr10) << endl;
    cout << "arr9 < arr10:\n" << (arr9 < arr10) << endl;

    // ========== 8. Array实际应用：图像处理模拟 ==========
    cout << "\n--- 8. Array实际应用：图像处理模拟 ---" << endl;

    // 模拟图像像素值（0-255）
    ArrayXXd image(3, 3);
    image << 100, 150, 200,
             120, 180, 220,
             80,  130, 190;

    cout << "原始图像像素值:\n" << image << endl;

    // 亮度调整（增加50）
    ArrayXXd brightened = image + 50;
    brightened = brightened.min(255.0);  // 限制在255
    cout << "亮度调整后:\n" << brightened << endl;

    // 对比度调整（乘以1.5）
    ArrayXXd contrasted = image * 1.5;
    contrasted = contrasted.min(255.0);
    cout << "对比度调整后:\n" << contrasted << endl;

    // 阈值处理（二值化）
    ArrayXXd binary = (image > 150.0).select(ArrayXXd::Constant(image.rows(), image.cols(), 255.0),
                                             ArrayXXd::Constant(image.rows(), image.cols(), 0.0));
    cout << "二值化后（阈值150）:\n" << binary << endl;

    // ========== 9. Array实际应用：信号处理模拟 ==========
    cout << "\n--- 9. Array实际应用：信号处理模拟 ---" << endl;

    // 生成正弦信号
    int n = 10;
    ArrayXd t = ArrayXd::LinSpaced(n, 0, 2*M_PI);
    ArrayXd signal = t.sin();

    cout << "时间点:\n" << t << endl;
    cout << "正弦信号:\n" << signal << endl;

    // 添加噪声
    ArrayXd noise = ArrayXd::Random(n) * 0.1;
    ArrayXd noisy_signal = signal + noise;
    cout << "加噪声后的信号:\n" << noisy_signal << endl;

    // 滤波（简单移动平均）
    ArrayXd filtered(n);
    filtered(0) = noisy_signal(0);
    for (int i = 1; i < n-1; ++i) {
        filtered(i) = (noisy_signal(i-1) + noisy_signal(i) + noisy_signal(i+1)) / 3.0;
    }
    filtered(n-1) = noisy_signal(n-1);
    cout << "滤波后的信号:\n" << filtered << endl;

    // ========== 10. Array高级操作 ==========
    cout << "\n--- 10. Array高级操作 ---" << endl;

    ArrayXd arr11(6);
    arr11 << 1, -2, 3, -4, 5, -6;

    cout << "原始数组:\n" << arr11 << endl;

    // 符号函数
    ArrayXd sign_arr = arr11.sign();
    cout << "符号函数:\n" << sign_arr << endl;

    // 取整
    ArrayXd arr12(5);
    arr12 << 1.3, 2.7, 3.1, 4.9, 5.5;
    cout << "原始数组:\n" << arr12 << endl;
    cout << "floor():\n" << arr12.floor() << endl;
    cout << "ceil():\n" << arr12.ceil() << endl;
    cout << "round():\n" << arr12.round() << endl;

    // 裁剪到范围
    ArrayXd arr13(5);
    arr13 << 0, 2, 5, 8, 10;
    cout << "原始数组:\n" << arr13 << endl;
    arr13 = arr13.max(1.0).min(7.0);  // 限制在[1, 7]
    cout << "裁剪到[1,7]:\n" << arr13 << endl;

    cout << "\n=== Array类型示例完成 ===" << endl;
    cin.get();
    return 0;
}
