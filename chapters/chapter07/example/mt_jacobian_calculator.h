#ifndef MT_JACOBIAN_CALCULATOR_H
#define MT_JACOBIAN_CALCULATOR_H

#include "mt_model.h"
#include "mt_forward_solver.h"
#include <vector>

/**
 * MT Jacobian计算器模块
 * 负责计算反演所需的Jacobian矩阵（灵敏度矩阵）
 */
namespace MT {

class JacobianCalculator {
public:
    /**
     * 构造函数
     * @param forwardSolver 正演求解器指针（必须有效）
     */
    explicit JacobianCalculator(ForwardSolver* forwardSolver);
    ~JacobianCalculator();

    /**
     * 计算Jacobian矩阵（有限差分扰动法）
     * @param m 当前模型参数（log10(ρ)）
     * @param omega 角频率数组
     * @param dSyn 当前合成数据
     * @param layerThicknesses 层厚度数组
     * @param epsilon 扰动步长
     * @param J 输出的Jacobian矩阵（nData行×M列）
     */
    void compute(const std::vector<double>& m,
                 const std::vector<double>& omega,
                 const std::vector<double>& dSyn,
                 const std::vector<double>& layerThicknesses,
                 double epsilon,
                 std::vector<std::vector<double>>& J);

    /**
     * 计算Jacobian矩阵（使用模型参数结构）
     * @param model 模型参数
     * @param omega 角频率数组
     * @param dSyn 当前合成数据
     * @param epsilon 扰动步长
     * @param J 输出的Jacobian矩阵
     */
    void compute(const ModelParams& model,
                 const std::vector<double>& omega,
                 const std::vector<double>& dSyn,
                 double epsilon,
                 std::vector<std::vector<double>>& J);

    /**
     * 设置扰动方法类型
     * @param method 方法类型（"forward" 或 "central"）
     */
    void setPerturbationMethod(const std::string& method);

private:
    ForwardSolver* m_forwardSolver;  // 正演求解器
    std::string m_perturbationMethod; // 扰动方法类型
};

} // namespace MT

#endif // MT_JACOBIAN_CALCULATOR_H

