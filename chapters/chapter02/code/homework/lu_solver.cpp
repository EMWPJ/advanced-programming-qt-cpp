#include "lu_solver.h"
#include <algorithm>
#include <cmath>

std::unique_ptr<Vector> LUSolver::solve(const Matrix& A_orig, const Vector& b_orig) {

    if (A_orig.is_singular()) {
        return nullptr; // 矩阵奇异，无法使用LU分解求解
    }

    size_t n = A_orig.rows();
    Matrix L(n, n, 0.0);
    Matrix U(n, n, 0.0);

    if (!lu_decomposition(A_orig, L, U)) {
        return nullptr; // LU分解失败
    }

    // 前代求解 Ly = b
    auto y = std::make_unique<Vector>(n);
    forward_substitution(L, b_orig, *y);

    // 回代求解 Ux = y
    auto x = std::make_unique<Vector>(n);
    back_substitution(U, *y, *x);

    return x;
}

bool LUSolver::lu_decomposition(const Matrix& A, Matrix& L, Matrix& U) {
    size_t n = A.rows();

    for (size_t i = 0; i < n; ++i) {
        // 计算U的第i行 - 使用Lambda表达式优化
        for (size_t j = i; j < n; ++j) {
            // 使用Lambda表达式计算前i个元素的和
            double sum = 0.0;
            for (size_t k = 0; k < i; ++k) {
                sum += L.at(i, k) * U.at(k, j);
            }
            U.at(i, j) = A.at(i, j) - sum;
        }

        // 计算L的第i列 - 使用Lambda表达式优化
        for (size_t j = i; j < n; ++j) {
            if (i == j) {
                L.at(i, i) = 1.0; // 对角元素设为1
            } else {
                // 使用Lambda表达式计算前i个元素的和
                double sum = 0.0;
                for (size_t k = 0; k < i; ++k) {
                    sum += L.at(j, k) * U.at(k, i);
                }
                if (std::abs(U.at(i, i)) < 1e-10) {
                    return false; // 矩阵奇异（LU分解中出现零主元）
                }
                L.at(j, i) = (A.at(j, i) - sum) / U.at(i, i);
            }
        }
    }
    return true;
}

void LUSolver::forward_substitution(const Matrix& L, const Vector& b, Vector& y) {
    size_t n = L.rows();

    // 前向替代 - 使用传统循环实现
    for (size_t i = 0; i < n; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < i; ++j) {
            sum += L.at(i, j) * y.at(j);
        }
        y.at(i) = (b.at(i) - sum) / L.at(i, i);
    }
}

void LUSolver::back_substitution(const Matrix& U, const Vector& y, Vector& x) {
    size_t n = U.rows();

    // 后向替代 - 使用传统循环实现
    for (int i = n - 1; i >= 0; --i) {
        double sum = 0.0;
        for (size_t j = i + 1; j < n; ++j) {
            sum += U.at(i, j) * x.at(j);
        }
        x.at(i) = (y.at(i) - sum) / U.at(i, i);
    }
}
