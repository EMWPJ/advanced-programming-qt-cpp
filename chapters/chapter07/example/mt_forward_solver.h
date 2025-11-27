#ifndef MT_FORWARD_SOLVER_H
#define MT_FORWARD_SOLVER_H

#include "mt_model.h"
#include <mkl.h>
#include <vector>

/**
 * MT正演求解器模块
 * 使用向上递推阻抗的解析法求解1D MT正演问题
 */
namespace MT {

class ForwardSolver {
public:
    ForwardSolver();
    ~ForwardSolver();

    /**
     * 执行正演计算
     * @param mLogRho 模型参数（log10(ρ)）
     * @param omega 角频率数组
     * @param layerThicknesses 层厚度数组
     * @param dataOut 输出的MT响应数据（log10(ρ_a)和相位）
     */
    void solve(const std::vector<double>& mLogRho,
               const std::vector<double>& omega,
               const std::vector<double>& layerThicknesses,
               std::vector<double>& dataOut);

    /**
     * 执行正演计算（使用模型参数结构）
     * @param model 模型参数
     * @param omega 角频率数组
     * @param dataOut 输出的MT响应数据
     */
    void solve(const ModelParams& model,
               const std::vector<double>& omega,
               std::vector<double>& dataOut);

private:
    /**
     * 计算电导率
     * @param mLogRho 模型参数（log10(ρ)）
     * @param sigma 输出的电导率数组
     */
    void computeConductivity(const std::vector<double>& mLogRho,
                             std::vector<double>& sigma);

    /**
     * 使用向上递推阻抗的解析法计算地表阻抗
     * @param M 模型层数
     * @param w 角频率
     * @param sigma 电导率数组
     * @param dz 层厚度数组
     * @param Z_surface 输出的地表阻抗
     */
    void computeRecursiveImpedance(int M, double w,
                                   const std::vector<double>& sigma,
                                   const std::vector<double>& dz,
                                   MKL_Complex16& Z_surface);
};

} // namespace MT

#endif // MT_FORWARD_SOLVER_H

