/**
 * @file vector.cpp
 * @brief 向量类的实现文件
 *
 * 包含所有向量操作的具体实现，演示：
 * - 构造函数的不同形式
 * - 数值计算算法（点积、范数等）
 * - 异常处理
 * - 数学概念的编程实现
 */

#include "vector.h"
#include <stdexcept>    // 用于异常处理
#include <algorithm>    // 用于算法函数
#include <cmath>        // 用于数学函数（sqrt等）
#include <numeric>      // 用于数值算法

/**
 * @brief 默认构造函数实现
 *
 * @param size 向量大小
 * @param value 初始值
 *
 * 算法：使用std::vector的构造函数初始化所有元素
 * 时间复杂度：O(size)
 * 空间复杂度：O(size)
 *
 * 演示：成员初始化列表的使用
 */
Vector::Vector(size_t size, double value) : m_size(size), m_data(size, value) {}

/**
 * @brief 拷贝构造函数实现
 *
 * @param data 输入的向量数据
 *
 * 演示：从标准库容器构造自定义对象
 */
Vector::Vector(const std::vector<double>& data) : m_size(data.size()), m_data(data) {}

/**
 * @brief 移动构造函数实现
 *
 * @param data 右值引用的向量数据
 *
 * 演示：
 * - C++11移动语义
 * - std::move的使用
 * - 性能优化（避免拷贝）
 */
Vector::Vector(std::vector<double>&& data) : m_size(data.size()), m_data(std::move(data)) {}

/**
 * @brief 元素访问函数（可修改版本）
 *
 * @param index 元素索引
 * @return 元素的引用
 *
 * 演示：
 * - 边界检查
 * - 异常处理（std::out_of_range）
 * - 返回引用允许修改元素
 * - const正确性
 */
double& Vector::at(size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("Vector index out of range");
    }
    return m_data[index];
}

/**
 * @brief 元素访问函数（只读版本）
 *
 * @param index 元素索引
 * @return 元素的值
 *
 * 演示：
 * - const正确性
 * - 边界检查
 * - 异常处理
 */
double Vector::at(size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("Vector index out of range");
    }
    return m_data[index];
}



/**
 * @brief 向量标量乘法实现
 *
 * @param scalar 标量值
 * @return 乘法结果
 *
 * 算法：元素级乘法
 * 原理：C[i] = A[i] * scalar
 * 时间复杂度：O(n)
 *
 * 数学性质：
 * - 结合律：(k*A) * m = k*(A*m)
 * - 分配律：k*(A + B) = k*A + k*B
 *
 * 演示：
 * - 变量捕获：Lambda表达式捕获scalar
 * - 闭包（closure）的使用
 */
Vector Vector::multiply(double scalar) const {
    std::vector<double> result_data(m_size);

    // 使用std::transform和Lambda表达式进行向量标量乘法
    std::transform(m_data.begin(), m_data.end(), result_data.begin(),
                   [scalar](double x) { return x * scalar; });

    return Vector(std::move(result_data));
}


/**
 * @brief 向量范数（2-范数）实现
 *
 * @return 向量的欧几里得长度
 *
 * 算法：sqrt(所有元素的平方和)
 * 原理：||A|| = sqrt(Σ(A[i]^2))
 * 时间复杂度：O(n)
 *
 * 数学性质：
 * - 非负性：||A|| ≥ 0
 * - 齐次性：||k*A|| = |k|*||A||
 * - 三角不等式：||A+B|| ≤ ||A|| + ||B||
 *
 * 几何意义：
 * - 向量到原点的距离
 * - 在数值分析中常用于度量误差
 *
 * 数值稳定性：
 * - 直接计算避免函数调用开销
 * - 避免浮点数精度损失
 */
double Vector::norm() const {
    // 使用std::accumulate和Lambda表达式计算平方和
    double sum_of_squares = std::accumulate(
        m_data.begin(), m_data.end(), 0.0,
        [](double acc, double x) { return acc + x * x; }
    );
    return std::sqrt(sum_of_squares);
}


/**
 * @brief 打印向量到控制台
 *
 * 演示：
 * - 格式化输出
 * - std::cout和std::flush的使用
 * - 循环控制和条件判断
 * - 字符串连接
 */
void Vector::print() const {
    // 使用std::for_each和Lambda表达式打印向量元素
    std::for_each(m_data.begin(), m_data.end(),
                  [](double x) { std::cout << x << " "; });
    std::cout << std::endl << std::flush;  // 换行并立即输出
}
