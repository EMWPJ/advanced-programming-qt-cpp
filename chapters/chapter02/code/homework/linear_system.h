/**
 * @file linear_system.h
 * @brief 线性方程组管理类 - 统一求解接口
 *
 * 设计理念：
 * - 门面模式：为复杂的求解系统提供统一接口
 * - 策略模式：支持多种求解算法
 * - 资源管理：智能指针确保内存安全
 *
 * 核心功能：
 * - 封装Ax=b形式的线性方程组
 * - 提供统一的求解接口
 * - 支持多种数值算法
 * - 优雅的错误处理机制
 */

#ifndef LINEAR_SYSTEM_H
#define LINEAR_SYSTEM_H

#include "matrix.h"           // 矩阵类
#include "vector.h"           // 向量类
#include "solver_base.h"      // 求解器基类
#include <memory>             // 智能指针
#include <vector>             // 向量容器
#include <numeric>            // 用于std::accumulate等数值算法
#include <algorithm>          // 用于std::transform等算法

/**
 * @class LinearSystem
 * @brief 线性方程组管理类 - Ax = b
 *
 * 设计模式：
 * - 门面模式：统一求解接口，隐藏复杂性
 * - 策略模式：可插拔的不同求解算法
 * - RAII：智能指针自动管理资源
 *
 * 核心特性：
 * - 封装系数矩阵A和常数向量b
 * - 提供统一的求解接口
 * - 支持多种数值算法（高斯消元、LU分解、迭代法等）
 * - 内置解的验证和精度分析
 * - 优雅的错误处理（nullptr返回）
 *
 * 使用示例：
 * @code
 * LinearSystem system(A, b);
 * auto x = system.solve(std::make_unique<GaussianSolver>());
 * system.set_exact_solution(x_exact);  // 设置解析解
 * auto residual = system.compute_residual(x);  // 计算残差
 * auto error = system.compute_error(x);        // 计算误差
 * @endcode
 */
class LinearSystem {
private:
    Matrix m_A;    // 系数矩阵A
    Vector m_b;    // 常数向量b
    std::unique_ptr<Vector> m_exact_solution;    // 解析解（精确解）
    std::unique_ptr<Vector> m_numerical_solution; // 数值解（计算解）

public:
    /**
     * @brief 构造函数
     * @param A 系数矩阵
     * @param b 常数向量
     *
     * 功能：
     * - 深拷贝矩阵和向量数据
     * - 建立线性方程组的内部表示
     * - 验证数据一致性
     *
     * @note 矩阵的行数必须等于向量的长度
     */
    LinearSystem(const Matrix& A, const Vector& b);


    /**
     * @brief 使用指定求解器求解Ax = b
     * @param solver 求解器智能指针
     * @return 解向量，如果求解失败返回nullptr
     *
     * 工作流程：
     * 1. 验证求解器有效性
     * 2. 检查求解器适用性
     * 3. 执行求解算法
     * 4. 自动保存数值解
     * 5. 返回计算结果
     *
     * @note 求解失败时返回nullptr（非异常设计）
     */
    std::unique_ptr<Vector> solve(std::unique_ptr<SolverBase> solver);

    /**
     * @brief 设置解析解（精确解）
     * @param exact_solution 解析解向量
     *
     * @note 用于后续的精度验证和误差分析
     */
    void set_exact_solution(std::unique_ptr<Vector> exact_solution);

    /**
     * @brief 设置数值解（计算解）
     * @param numerical_solution 数值解向量
     *
     * @note 通常通过solve()方法自动设置，也可手动设置
     */
    void set_numerical_solution(std::unique_ptr<Vector> numerical_solution);

    /**
     * @brief 获取解析解
     * @return 解析解向量，如果未设置返回nullptr
     */
    const Vector* get_exact_solution() const;

    /**
     * @brief 获取数值解
     * @return 数值解向量，如果未设置返回nullptr
     */
    const Vector* get_numerical_solution() const;

    /**
     * @brief 计算给定解的残差 ||Ax - b||
     * @param x 解向量
     * @return 残差范数
     *
     * 数学定义：
     * residual = ||Ax - b||
     *
     * @note 残差越小，解的准确性越高
     */
    double compute_residual(const Vector& x) const;

    /**
     * @brief 计算数值解与解析解的误差 ||x_numerical - x_exact||
     * @return 误差范数，如果未设置解析解返回-1.0
     *
     * 数学定义：
     * error = ||x_numerical - x_exact||
     *
     * @note 需要先设置解析解和数值解
     */
    double compute_error() const;

    /**
     * @brief 验证解的准确性
     * @param x 待验证的解向量
     * @param tolerance 容差阈值（默认1e-6）
     * @return 验证结果：true表示解准确，false表示解不准确
     *
     * 验证标准：
     * - 计算残差 ||Ax - b||
     * - 检查残差是否小于容差阈值
     * - 同时考虑相对误差和绝对误差
     */
    bool validate_solution(const Vector& x, double tolerance = 1e-6) const;

    /**
     * @brief 综合精度分析报告
     *
     * 输出内容：
     * - 残差分析
     * - 误差分析（如果有解析解）
     * - 数值稳定性评估
     * - 算法收敛性分析
     */
    void print_accuracy_report() const;


    /**
     * @brief 打印线性方程组Ax = b
     *
     * 输出格式：
     * @code
     * Coefficient Matrix A:
     * 5 1 0 1 0
     * 1 4 1 0 0
     * ...
     * Constant Vector b:
     * 8 7 9 6 7
     * @endcode
     *
     * @note 主要用于调试和结果展示
     */
    void print() const;
};

#endif // LINEAR_SYSTEM_H
