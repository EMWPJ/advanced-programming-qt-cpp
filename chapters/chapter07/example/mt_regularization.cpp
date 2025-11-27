#include "mt_regularization.h"
#include <algorithm>
#include <mkl_cblas.h>

namespace MT {

Regularization::Regularization()
    : m_type(Type::SMOOTHNESS) {
}

Regularization::~Regularization() {
}

void Regularization::buildLMatrix(int M, Type type, std::vector<std::vector<double>>& L) {
    m_type = type;
    buildLMatrix(M, L);
}

void Regularization::buildLMatrix(int M, std::vector<std::vector<double>>& L) {
    switch (m_type) {
        case Type::SMOOTHNESS:
            buildSmoothnessMatrix(M, L);
            break;
        case Type::FLATNESS:
            buildFlatnessMatrix(M, L);
            break;
        case Type::MINIMUM_NORM:
            buildMinimumNormMatrix(M, L);
            break;
    }
}

void Regularization::computeLTL(const std::vector<std::vector<double>>& L,
                                std::vector<std::vector<double>>& LTL) {
    int L_rows = static_cast<int>(L.size());
    int M = static_cast<int>(L[0].size());

    // 将L转换为列主序的连续矩阵 (col-major, size L_rows x M)
    std::vector<double> Lmat(L_rows * M);
    for (int i = 0; i < L_rows; ++i) {
        for (int j = 0; j < M; ++j) {
            Lmat[j * L_rows + i] = L[i][j]; // col-major: (i,j) -> j*L_rows + i
        }
    }

    // 构造输出矩阵LTL (M x M, col-major顺序)
    std::vector<double> LTLmat(M * M, 0.0);

    // LTL = L^T * L, 即 (M x L_rows) * (L_rows x M) = (M x M)
    // 注意：L的维度为 (L_rows, M)
    cblas_dgemm(
        CblasColMajor,          // 列主序
        CblasTrans,             // A^T（L^T）
        CblasNoTrans,           // B
        M,                      // M (L^T: M x L_rows)
        M,                      // N
        L_rows,                 // K
        1.0,                    // alpha
        Lmat.data(),            // A = L
        L_rows,                 // lda
        Lmat.data(),            // B = L
        L_rows,                 // ldb
        0.0,                    // beta
        LTLmat.data(),          // C
        M                       // ldc
    );

    // 组装成std::vector<std::vector<double>> (按行填充)
    LTL.resize(M);
    for (int i = 0; i < M; ++i) {
        LTL[i].resize(M);
        for (int j = 0; j < M; ++j) {
            // col-major：元素(i,j)在LTLmat中为 j*M + i
            LTL[i][j] = LTLmat[j * M + i];
        }
    }
}

void Regularization::setType(Type type) {
    m_type = type;
}

void Regularization::buildSmoothnessMatrix(int M, std::vector<std::vector<double>>& L) {
    int L_rows = M - 2;  // 二阶差分矩阵有M-2行
    L.resize(L_rows);
    for (int i = 0; i < L_rows; i++) {
        L[i].resize(M, 0.0);
        L[i][i] = 1.0;
        L[i][i + 1] = -2.0;
        L[i][i + 2] = 1.0;
    }
}

void Regularization::buildFlatnessMatrix(int M, std::vector<std::vector<double>>& L) {
    int L_rows = M - 1;  // 一阶差分矩阵有M-1行
    L.resize(L_rows);
    for (int i = 0; i < L_rows; i++) {
        L[i].resize(M, 0.0);
        L[i][i] = -1.0;
        L[i][i + 1] = 1.0;
    }
}

void Regularization::buildMinimumNormMatrix(int M, std::vector<std::vector<double>>& L) {
    // 单位矩阵
    L.resize(M);
    for (int i = 0; i < M; i++) {
        L[i].resize(M, 0.0);
        L[i][i] = 1.0;
    }
}

} // namespace MT

