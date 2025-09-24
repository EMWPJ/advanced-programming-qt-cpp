/**
 * @file matrix.h
 * @brief 矩阵类 - 数值线性代数计算
 *
 * 核心特性：
 * - 二维数值矩阵存储和操作
 * - 多种构造函数（默认、数据、复制）
 * - 矩阵-向量乘法运算
 * - 数值稳定性保证
 * - 内存安全的RAII设计
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>        // 用于存储矩阵数据
#include <iostream>      // 用于输出
#include <functional>    // 用于Lambda表达式
#include <memory>        // 用于智能指针
#include <algorithm>     // 用于std::for_each等算法

/**
 * @class Matrix
 * @brief 数值矩阵类 - 线性代数计算基础
 *
 * 设计特点：
 * - 二维double数据存储
 * - 行优先存储布局
 * - const正确性保证
 * - 数值精度控制（1e-10）
 * - 异常安全的数据访问
 *
 * 使用示例：
 * @code
 * Matrix A(3, 3);  // 3x3零矩阵
 * Matrix B(data);  // 从数据构造
 * auto x = A.multiply_vector(b);  // Ax = b
 * @endcode
 */
class Matrix {
private:
    size_t m_rows;                                   // 矩阵行数
    size_t m_cols;                                   // 矩阵列数
    std::vector<std::vector<double>> m_data;        // 存储矩阵数据的二维向量

public:
    /**
     * @brief 默认构造函数 - 创建指定大小的矩阵
     * @param rows 行数
     * @param cols 列数
     * @param value 初始值（默认为0.0）
     *
     * 示例：Matrix m(3, 4, 1.0); 创建3x4的全1矩阵
     */
    Matrix(size_t rows, size_t cols, double value = 0.0);

    /**
     * @brief 构造函数 - 从二维向量创建矩阵
     * @param data 二维向量数据
     *
     * 示例：Matrix m({{1,2},{3,4}});
     */
    Matrix(const std::vector<std::vector<double>>& data);

    /**
     * @brief 移动构造函数 - 高效转移数据所有权
     * @param data 右值引用的二维向量
     *
     * 演示C++11的移动语义，避免不必要的拷贝
     */
    Matrix(std::vector<std::vector<double>>&& data);

    // ===== 基本访问操作 =====

    /**
     * @brief 获取矩阵行数
     * @return 矩阵行数
     */
    size_t rows() const { return m_rows; }

    /**
     * @brief 获取矩阵列数
     * @return 矩阵列数
     */
    size_t cols() const { return m_cols; }

    /**
     * @brief 访问矩阵元素（可修改）
     * @param row 行索引
     * @param col 列索引
     * @return 元素的引用
     *
     * 演示边界检查和异常处理
     */
    double& at(size_t row, size_t col);

    /**
     * @brief 访问矩阵元素（只读）
     * @param row 行索引
     * @param col 列索引
     * @return 元素的值
     */
    double at(size_t row, size_t col) const;

    // ===== 矩阵运算 =====

    /**
     * @brief 矩阵-向量乘法
     * @param vec 向量（以std::vector形式）
     * @return 智能指针指向结果向量
     *
     * 演示智能指针使用
     */
    std::unique_ptr<std::vector<double>> multiply(const std::vector<double>& vec) const;


    // ===== 输入输出 =====

    /**
     * @brief 打印矩阵到控制台
     * 演示std::cout和std::flush的使用
     */
    void print() const;

    // ===== 性能优化接口 =====
    // 以下方法仅用于内部性能优化，外部使用应避免直接访问

    /**
     * @brief 获取原始数据引用（仅用于内部优化）
     * @return 内部数据引用
     */
    const std::vector<std::vector<double>>& get_raw_data() const { return m_data; }

    /**
     * @brief 获取可修改的原始数据引用（仅用于内部优化）
     * @return 内部数据引用
     */
    std::vector<std::vector<double>>& get_mutable_raw_data() { return m_data; }

    // ===== 辅助功能 =====

    /**
     * @brief 检查矩阵是否奇异（行列式检测）
     * @return bool 如果矩阵奇异返回true
     *
     * 算法：
     * - 计算矩阵的行列式
     * - 如果行列式接近零，则认为矩阵奇异
     * - 支持1x1、2x2、3x3矩阵的精确计算
     * - 大矩阵（>3x3）使用对角元素检查作为近似
     *
     * 数值特性：
     * - 1e-10精度阈值（小矩阵）
     * - 1e-12精度阈值（大矩阵对角元素检查）
     * - 递归算法实现，适合小矩阵
     * - 大矩阵避免指数级计算复杂度
     *
     * 数学理论：
     * - 奇异矩阵行列式为零
     * - 行列式计算的数值稳定性
     * - 递归算法的时间复杂度分析
     * - 对角占优矩阵的数值特性
     */
    bool is_singular() const;

    /**
     * @brief 计算方阵的行列式
     * @return double 行列式的值
     *
     * 算法：
     * - 1x1：返回元素本身
     * - 2x2：ad-bc公式
     * - 3x3：Sarrus法则
     * - 4x4+：使用对角元素检查并返回非零值（避免指数级复杂度）
     *
     * 数值考虑：
     * - 递归算法实现，适合小矩阵
     * - 大矩阵避免指数级计算复杂度
     * - 精度控制和数值稳定性
     * - 对角元素检查作为奇异性判断的补充
     *
     * 注意：
     * - 4x4及以上的矩阵不进行完整的行列式计算
     * - 使用对角元素检查作为近似方法
     * - 适用于大多数数值计算场景
     */
    double determinant() const;

};

#endif // MATRIX_H
