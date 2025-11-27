#include "mt_optimizer.h"
#include <mkl.h>
#include <mkl_lapacke.h>
#include <mkl_blas.h>
#include <cmath>
#include <stdexcept>
#include <limits>

namespace MT {

Optimizer::Optimizer()
    : m_solverType("cholesky") {
}

Optimizer::~Optimizer() {
}

bool Optimizer::solve(const std::vector<std::vector<double>>& JTJ,
                      const std::vector<std::vector<double>>& LTL,
                      double lambda,
                      const std::vector<double>& JTr,
                      std::vector<double>& dm) {
    int M = static_cast<int>(JTr.size());

    // 检查矩阵维度
    if (M <= 0 || JTJ.size() != static_cast<size_t>(M) || LTL.size() != static_cast<size_t>(M)) {
        return false;
    }
    
    // 检查lambda有效性
    if (!std::isfinite(lambda) || lambda < 0.0) {
        return false;
    }
    
    // 检查JTr是否包含NaN或Inf
    for (int i = 0; i < M; i++) {
        if (!std::isfinite(JTr[i])) {
            return false;
        }
    }

    // 构建正规方程矩阵：A_reg = JTJ + λ*LTL，使用BLAS进行矩阵加法

    // 1. 检查行列数有效性
    for (int i = 0; i < M; i++) {
        if (JTJ[i].size() != static_cast<size_t>(M) || LTL[i].size() != static_cast<size_t>(M)) {
            return false;
        }
    }

    // 2. 拍平成一维row-major数组
    std::vector<double> JTJ_flat(M * M), LTL_flat(M * M);
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            JTJ_flat[i * M + j] = JTJ[i][j];
            LTL_flat[i * M + j] = LTL[i][j];
        }
    }

    // 3. A_reg_flat = JTJ_flat + lambda * LTL_flat (调用BLAS cblas_daxpy)
    std::vector<double> A_reg_flat = JTJ_flat;
    if (!std::isfinite(lambda) || lambda < 0.0) return false;

    // DAXPY: A = A + lambda * B
    cblas_daxpy(M * M, lambda, LTL_flat.data(), 1, A_reg_flat.data(), 1);

    // 4. 检查NaN/Inf
    for (int idx = 0; idx < M * M; idx++) {
        if (!std::isfinite(A_reg_flat[idx])) return false;
    }

    // 右端项
    dm = JTr;

    // 求解正规方程
    int info = 0;
    if (m_solverType == "cholesky") {
        // 使用Cholesky分解（对称正定矩阵）
        info = LAPACKE_dposv(LAPACK_ROW_MAJOR, 'L', M, 1,
                             A_reg_flat.data(), M, dm.data(), 1);
    } else if (m_solverType == "lu") {
        // 使用LU分解（通用矩阵）
        std::vector<int> ipiv(M);
        info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, M, 1,
                             A_reg_flat.data(), M, ipiv.data(), dm.data(), 1);
    }

    return (info == 0);
}

void Optimizer::computeJTJ(const std::vector<std::vector<double>>& J,
                           std::vector<std::vector<double>>& JTJ) {
    int nData = static_cast<int>(J.size());
    int M = static_cast<int>(J[0].size());

    // 将J转换为行主序数组
    std::vector<double> J_flat(nData * M);
    for (int i = 0; i < nData; i++) {
        for (int j = 0; j < M; j++) {
            J_flat[i * M + j] = J[i][j];
        }
    }

    // 计算JTJ（使用cblas_dsyrk）
    std::vector<double> JTJ_flat(M * M, 0.0);
    cblas_dsyrk(CblasRowMajor, CblasUpper, CblasTrans,
                M,      // 结果矩阵的阶数
                nData,  // J^T的列数（J的行数）
                1.0,    // alpha
                J_flat.data(), M,  // J矩阵（行主序，nData×M）
                0.0,    // beta
                JTJ_flat.data(), M);  // 结果矩阵（行主序，M×M，上三角）

    // 填充下三角（对称矩阵）
    for (int i = 0; i < M; i++) {
        for (int j = i + 1; j < M; j++) {
            JTJ_flat[j * M + i] = JTJ_flat[i * M + j];
        }
    }

    // 转换为二维数组
    JTJ.resize(M);
    for (int i = 0; i < M; i++) {
        JTJ[i].resize(M);
        for (int j = 0; j < M; j++) {
            JTJ[i][j] = JTJ_flat[i * M + j];
        }
    }
}

void Optimizer::computeJTr(const std::vector<std::vector<double>>& J,
                           const std::vector<double>& r,
                           std::vector<double>& JTr) {
    int nData = static_cast<int>(J.size());
    int M = static_cast<int>(J[0].size());

    // 将J转换为行主序数组
    std::vector<double> J_flat(nData * M);
    for (int i = 0; i < nData; i++) {
        for (int j = 0; j < M; j++) {
            J_flat[i * M + j] = J[i][j];
        }
    }

    // 计算JTr（使用cblas_dgemv）
    JTr.resize(M, 0.0);
    cblas_dgemv(CblasRowMajor, CblasTrans,
                nData, M, 1.0,
                J_flat.data(), M,
                r.data(), 1,
                0.0, JTr.data(), 1);
}

void Optimizer::setSolverType(const std::string& type) {
    if (type == "cholesky" || type == "lu") {
        m_solverType = type;
    } else {
        throw std::invalid_argument("Solver type must be 'cholesky' or 'lu'");
    }
}

} // namespace MT

