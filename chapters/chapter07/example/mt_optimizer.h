#ifndef MT_OPTIMIZER_H
#define MT_OPTIMIZER_H

#include "mt_model.h"
#include <vector>

/**
 * MT优化求解器模块
 * 负责求解反演中的正规方程
 */
namespace MT {

class Optimizer {
public:
    Optimizer();
    ~Optimizer();

    /**
     * 求解正规方程：(J^T*J + λ*L^T*L) * δm = J^T*r
     * @param JTJ J^T*J矩阵（M×M对称矩阵）
     * @param LTL L^T*L矩阵（M×M对称矩阵）
     * @param lambda 正则化参数
     * @param JTr J^T*r向量（M维）
     * @param dm 输出的模型更新向量（M维）
     * @return 是否成功
     */
    bool solve(const std::vector<std::vector<double>>& JTJ,
               const std::vector<std::vector<double>>& LTL,
               double lambda,
               const std::vector<double>& JTr,
               std::vector<double>& dm);

    /**
     * 计算J^T*J（使用MKL BLAS）
     * @param J Jacobian矩阵（nData行×M列）
     * @param JTJ 输出的J^T*J矩阵（M×M对称矩阵）
     */
    void computeJTJ(const std::vector<std::vector<double>>& J,
                    std::vector<std::vector<double>>& JTJ);

    /**
     * 计算J^T*r（使用MKL BLAS）
     * @param J Jacobian矩阵（nData行×M列）
     * @param r 残差向量（nData维）
     * @param JTr 输出的J^T*r向量（M维）
     */
    void computeJTr(const std::vector<std::vector<double>>& J,
                    const std::vector<double>& r,
                    std::vector<double>& JTr);

    /**
     * 设置求解器类型
     * @param type 求解器类型（"cholesky" 或 "lu"）
     */
    void setSolverType(const std::string& type);

private:
    std::string m_solverType;  // 求解器类型
};

} // namespace MT

#endif // MT_OPTIMIZER_H

