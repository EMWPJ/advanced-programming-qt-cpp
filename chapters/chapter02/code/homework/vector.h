/**
 * @file vector.h
 * @brief 数值向量类 - 一维数值计算
 *
 * 核心特性：
 * - 一维double数据存储和操作
 * - 多种构造函数（默认、数据、复制）
 * - 向量基本运算（加法、减法、标量乘法）
 * - 数值分析方法（范数计算）
 * - 内存安全的RAII设计
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <vector>        // 用于存储向量数据
#include <iostream>      // 用于输出
#include <functional>    // 用于Lambda表达式
#include <memory>        // 用于智能指针（可能需要扩展）
#include <numeric>       // 用于std::accumulate等数值算法

/**
 * @class Vector
 * @brief 数值向量类 - 一维线性代数
 *
 * 设计特点：
 * - 一维double数据存储
 * - 边界检查和异常安全
 * - const正确性保证
 * - 数值精度控制（1e-10）
 * - 高效的内存布局
 *
 * 数学概念：
 * - 向量：有序的数值集合
 * - 范数：向量的"长度"（2-范数）
 * - 标量乘法：向量与常数的乘积
 */
class Vector {
private:
    size_t m_size;                           // 向量维数
    std::vector<double> m_data;              // 存储向量元素的动态数组

public:
    /**
     * @brief 默认构造函数 - 创建指定大小的向量
     * @param size 向量大小
     * @param value 初始值（默认为0.0）
     *
     * 示例：Vector v(5, 1.0); 创建5维全1向量
     */
    Vector(size_t size, double value = 0.0);

    /**
     * @brief 构造函数 - 从std::vector创建向量
     * @param data 输入的向量数据
     *
     * 演示：从标准库容器创建自定义对象
     */
    Vector(const std::vector<double>& data);

    /**
     * @brief 移动构造函数 - 高效转移数据所有权
     * @param data 右值引用的向量数据
     *
     * 演示：移动语义，避免不必要的拷贝
     */
    Vector(std::vector<double>&& data);

    // ===== 基本访问操作 =====

    /**
     * @brief 获取向量大小
     * @return 向量维数
     */
    size_t size() const { return m_size; }

    /**
     * @brief 访问向量元素（可修改）
     * @param index 元素索引
     * @return 元素的引用
     *
     * 演示：边界检查和异常处理
     */
    double& at(size_t index);

    /**
     * @brief 访问向量元素（只读）
     * @param index 元素索引
     * @return 元素的值
     */
    double at(size_t index) const;

    // ===== 向量运算 =====



    /**
     * @brief 向量标量乘法
     * @param scalar 标量值
     * @return 乘法结果
     *
     * 算法：元素级乘法
     * 原理：C[i] = A[i] * scalar
     */
    Vector multiply(double scalar) const;


    /**
     * @brief 向量范数（2-范数）
     * @return 向量的欧几里得长度
     *
     * 数学定义：||A|| = sqrt(Σ(A[i]^2))
     * 几何意义：原点到向量的距离
     *
     * 演示：数值稳定性的考虑
     */
    double norm() const;

    // ===== Lambda表达式支持 =====

    // ===== 兼容性接口 =====

    /**
     * @brief 转换为std::vector
     * @return std::vector<double>格式的数据
     *
     * 演示：与标准库的互操作性
     */
    std::vector<double> to_std_vector() const { return m_data; }

    // ===== 性能优化接口 =====
    // 以下方法仅用于内部性能优化，外部使用应避免直接访问

    /**
     * @brief 获取原始数据引用（仅用于内部优化）
     * @return 内部数据引用
     */
    const std::vector<double>& get_raw_data() const { return m_data; }

    // ===== 输入输出 =====

    /**
     * @brief 打印向量到控制台
     * 演示：格式化输出和流操作
     */
    void print() const;
};

#endif // VECTOR_H
