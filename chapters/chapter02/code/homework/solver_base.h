/**
 * @file solver_base.h
 * @brief 线性方程组求解器基类 - 演示面向对象多态性
 *
 * 这个基类展示了：
 * - 抽象基类（ABC）的设计模式
 * - 纯虚函数定义接口
 * - 多态性（polymorphism）的使用
 * - 智能指针的返回值
 * - 简化的接口设计
 */

#ifndef SOLVER_BASE_H
#define SOLVER_BASE_H

#include "matrix.h"     // 矩阵类
#include "vector.h"     // 向量类
#include <memory>       // 智能指针

/**
 * @class SolverBase
 * @brief 线性方程组求解器抽象基类
 *
 * 设计模式：
 * - 模板方法模式：定义求解算法的骨架
 * - 策略模式：不同求解算法的封装
 * - 工厂模式：创建具体求解器实例
 *
 * 数学背景：
 * - 求解形如 Ax = b 的线性方程组
 * - A为系数矩阵，b为常数向量，x为未知向量
 * - 不同算法适用于不同类型的矩阵
 *
 * 演示：
 * - 纯虚函数定义接口
 * - 虚析构函数确保正确清理
 * - const正确性
 */
class SolverBase {
public:
    /**
     * @brief 虚析构函数
     *
     * 重要性：
     * - 确保通过基类指针删除派生类对象时正确调用派生类析构函数
     * - 防止内存泄漏
     * - C++多态性的基本要求
     *
     * 演示：= default 的使用
     */
    virtual ~SolverBase() = default;

    /**
     * @brief 求解线性方程组（纯虚函数）
     *
     * @param A 系数矩阵
     * @param b 常数向量
     * @return 智能指针指向解向量，如果求解失败返回nullptr
     *
     * 设计决策：
     * - 返回std::unique_ptr<Vector>：自动内存管理
     * - 返回nullptr表示求解失败：避免异常，提高性能
     * - const方法：不修改输入参数
     *
     * 数学定义：
     * - 寻找向量x，使得 A * x = b
     * - 如果A不可逆，则无解或无穷多解
     */
    virtual std::unique_ptr<Vector> solve(const Matrix& A, const Vector& b) = 0;

};

#endif // SOLVER_BASE_H
