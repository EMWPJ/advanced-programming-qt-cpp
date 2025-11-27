#include "mt_jacobian_calculator.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <limits>

namespace MT {

JacobianCalculator::JacobianCalculator(ForwardSolver* forwardSolver)
    : m_forwardSolver(forwardSolver), m_perturbationMethod("forward") {
    if (!forwardSolver) {
        throw std::invalid_argument("ForwardSolver pointer cannot be null");
    }
}

JacobianCalculator::~JacobianCalculator() {
}

void JacobianCalculator::compute(const std::vector<double>& m,
                                 const std::vector<double>& omega,
                                 const std::vector<double>& dSyn,
                                 const std::vector<double>& layerThicknesses,
                                 double epsilon,
                                 std::vector<std::vector<double>>& J) {
    int M = static_cast<int>(m.size());
    int nData = static_cast<int>(dSyn.size());

    J.resize(nData);
    for (int i = 0; i < nData; i++) {
        J[i].resize(M);
    }

    std::vector<double> mPerturbed = m;
    std::vector<double> dPerturbed;

    if (m_perturbationMethod == "forward") {
        // 前向差分法
        for (int j = 0; j < M; j++) {
            // 扰动第j个参数
            mPerturbed[j] = m[j] + epsilon;

            // 正演计算扰动后的数据
            m_forwardSolver->solve(mPerturbed, omega, layerThicknesses, dPerturbed);

            // 计算有限差分：J[:,j] = (d_perturbed - d_syn) / epsilon
            // 检查epsilon是否为零或过小
            if (epsilon <= 0.0 || !std::isfinite(epsilon)) {
                throw std::runtime_error("Jacobian计算错误：epsilon必须为正数且有限");
            }
            for (int i = 0; i < nData; i++) {
                double diff = dPerturbed[i] - dSyn[i];
                J[i][j] = diff / epsilon;
                // 检查结果是否为NaN或Inf
                if (!std::isfinite(J[i][j])) {
                    J[i][j] = 0.0;  // 如果计算失败，设为0
                }
            }

            // 恢复参数
            mPerturbed[j] = m[j];
        }
    } else if (m_perturbationMethod == "central") {
        // 中心差分法（更精确但需要两次正演）
        std::vector<double> dPerturbedNeg;
        for (int j = 0; j < M; j++) {
            // 正向扰动
            mPerturbed[j] = m[j] + epsilon;
            m_forwardSolver->solve(mPerturbed, omega, layerThicknesses, dPerturbed);

            // 负向扰动
            mPerturbed[j] = m[j] - epsilon;
            m_forwardSolver->solve(mPerturbed, omega, layerThicknesses, dPerturbedNeg);

            // 计算中心差分：J[:,j] = (d_perturbed_pos - d_perturbed_neg) / (2*epsilon)
            // 检查epsilon是否为零或过小
            double denom = 2.0 * epsilon;
            if (denom <= 0.0 || !std::isfinite(denom)) {
                throw std::runtime_error("Jacobian计算错误：epsilon必须为正数且有限");
            }
            for (int i = 0; i < nData; i++) {
                double diff = dPerturbed[i] - dPerturbedNeg[i];
                J[i][j] = diff / denom;
                // 检查结果是否为NaN或Inf
                if (!std::isfinite(J[i][j])) {
                    J[i][j] = 0.0;  // 如果计算失败，设为0
                }
            }

            // 恢复参数
            mPerturbed[j] = m[j];
        }
    }
}

void JacobianCalculator::compute(const ModelParams& model,
                                 const std::vector<double>& omega,
                                 const std::vector<double>& dSyn,
                                 double epsilon,
                                 std::vector<std::vector<double>>& J) {
    compute(model.mLogRho, omega, dSyn, model.layerThicknesses, epsilon, J);
}

void JacobianCalculator::setPerturbationMethod(const std::string& method) {
    if (method == "forward" || method == "central") {
        m_perturbationMethod = method;
    } else {
        throw std::invalid_argument("Perturbation method must be 'forward' or 'central'");
    }
}

} // namespace MT

