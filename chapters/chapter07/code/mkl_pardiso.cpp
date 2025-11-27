#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <windows.h>

// Intel MKL Pardiso头文件
#include <mkl.h>
#include <mkl_pardiso.h>

using namespace std;
using namespace chrono;

void print_vector(const char* name, const double* vector, int size) {
    cout << name << ":" << endl;
    for (int i = 0; i < size; ++i) {
        cout << vector[i] << " ";
    }
    cout << endl << endl;
}

void print_matrix_csr(const char* name, const int* row_index, const int* col_index,
                      const double* values, int n, int nnz, bool fortran_indexing = true) {
    cout << name << " (CSR格式):" << endl;
    cout << "行索引: ";
    for (int i = 0; i <= n; ++i) {
        cout << row_index[i] << " ";
    }
    cout << endl;
    cout << "列索引: ";
    for (int i = 0; i < nnz; ++i) {
        cout << col_index[i] << " ";
    }
    cout << endl;
    cout << "非零值: ";
    for (int i = 0; i < nnz; ++i) {
        cout << values[i] << " ";
    }
    cout << endl << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL Pardiso 稀疏矩阵求解器示例 ===" << endl << endl;

    // ========== 示例1: 对称矩阵求解（基于官方示例） ==========
    cout << "=== 示例1: 对称矩阵求解（官方示例风格） ===" << endl;

    // 使用官方示例中的矩阵数据（8x8对称矩阵）
    const int n = 8;
    vector<int> ia = {1, 5, 8, 10, 12, 15, 17, 18, 19};  // Fortran风格索引
    vector<int> ja = {
        1,    3,       6, 7,      // 第1行
             2, 3,    5,         // 第2行
                3,             8, // 第3行
                   4,       7,    // 第4行
                      5, 6, 7,    // 第5行
                         6,    8, // 第6行
                            7,    // 第7行
                               8  // 第8行
    };
    vector<double> a = {
        7.0,      1.0,           2.0, 7.0,  // 第1行
              -4.0, 8.0,      2.0,         // 第2行
                    1.0,                     5.0,  // 第3行
                         7.0,           9.0,        // 第4行
                              5.0, 1.0, 5.0,        // 第5行
                                  -1.0,      5.0,    // 第6行
                                       11.0,         // 第7行
                                             5.0     // 第8行
    };

    // 右端向量 b = [1, 1, 1, 1, 1, 1, 1, 1]
    vector<double> b(n, 1.0);
    vector<double> x(n, 0.0);

    print_matrix_csr("稀疏矩阵A", ia.data(), ja.data(), a.data(), n, ja.size(), true);
    print_vector("右端向量b", b.data(), n);

    // Pardiso参数
    void* pt[64];  // 内部求解器内存指针数组
    int iparm[64];  // 控制参数数组
    int maxfct = 1;  // 最大因子数
    int mnum = 1;  // 使用的因子编号
    int mtype = -2;  // 矩阵类型: -2=实对称不定矩阵（官方示例使用）
    int phase;  // 求解阶段
    int nrhs = 1;  // 右端向量数量
    int msglvl = 1;  // 消息级别
    int error = 0;  // 错误代码

    // 初始化Pardiso
    for (int i = 0; i < 64; ++i) {
        pt[i] = nullptr;
        iparm[i] = 0;
    }

    // 设置Pardiso参数（基于官方示例）
    iparm[0] = 1;         /* 不使用求解器默认值 */
    iparm[1] = 2;         /* 填充减少重排序来自METIS */
    iparm[3] = 0;         /* 无迭代-直接算法 */
    iparm[4] = 0;         /* 无用户填充减少排列 */
    iparm[5] = 0;         /* 解写入x */
    iparm[6] = 0;         /* 未使用 */
    iparm[7] = 2;         /* 最大迭代精化步骤数 */
    iparm[8] = 0;         /* 未使用 */
    iparm[9] = 13;        /* 用1E-13扰动主元元素 */
    iparm[10] = 1;        /* 使用非对称排列和缩放MPS */
    iparm[11] = 0;        /* 未使用 */
    iparm[12] = 0;        /* 最大加权匹配算法被关闭（对称矩阵的默认值） */
    iparm[13] = 0;        /* 输出：扰动主元的数量 */
    iparm[14] = 0;        /* 未使用 */
    iparm[15] = 0;        /* 未使用 */
    iparm[16] = 0;        /* 未使用 */
    iparm[17] = -1;       /* 输出：LU因子中的非零元素数 */
    iparm[18] = -1;       /* 输出：LU分解的MFLOPS */
    iparm[19] = 0;        /* 输出：CG迭代次数 */
    iparm[26] = 0;        /* 0=无矩阵检查器，1=运行矩阵检查器 */
    iparm[27] = 0;        /* 0=双精度，1=单精度 */
    iparm[34] = 0;        /* 0=Fortran索引（1开始），1=C索引（0开始） */
    iparm[35] = 0;        /* 0=Fortran接口，1=C接口 */
    iparm[36] = 0;        /* 0=CSR格式，1=CSC格式 */
    iparm[59] = 0;        /* 0=核心内模式，1=核心内和核心外之间的自动切换 */

    // 阶段1: 符号分解
    cout << "=== 阶段1: 符号分解 ===" << endl;
    phase = 11;
    auto start = high_resolution_clock::now();

    error = 0;
    pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
            a.data(), ia.data(), ja.data(),
            nullptr, &nrhs, iparm, &msglvl, nullptr, nullptr, &error);

    if (error != 0) {
        cout << "符号分解错误，错误代码: " << error << endl;
        return 1;
    }

    cout << "符号分解完成..." << endl;
    cout << "LU中的非零元素数 = " << iparm[17] << endl;
    cout << "分解的MFLOPS = " << iparm[18] << endl;

    // 阶段2: 数值分解
    cout << "=== 阶段2: 数值分解 ===" << endl;
    phase = 22;

    pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
            a.data(), ia.data(), ja.data(),
            nullptr, &nrhs, iparm, &msglvl, nullptr, nullptr, &error);

    if (error != 0) {
        cout << "数值分解错误，错误代码: " << error << endl;
        return 1;
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout << "分解完成，耗时: " << duration.count() << " 微秒" << endl << endl;

    // 阶段3: 求解
    cout << "=== 阶段3: 求解 ===" << endl;
    phase = 33;
    iparm[7] = 2;  // 最大迭代精化步骤数

    pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
            a.data(), ia.data(), ja.data(),
            nullptr, &nrhs, iparm, &msglvl, b.data(), x.data(), &error);

    if (error != 0) {
        cout << "求解错误，错误代码: " << error << endl;
        return 1;
    }

    cout << "求解完成..." << endl;
    cout << "系统的解: " << endl;
    print_vector("x", x.data(), n);

    // 验证解
    cout << "=== 验证解 ===" << endl;
    vector<double> Ax(n, 0.0);
    vector<double> residual(n, 0.0);

    // 构造完整对称矩阵进行验证
    vector<vector<double>> full_matrix(n, vector<double>(n, 0.0));

    // 填充下三角部分（包括对角线）
    for (int i = 0; i < n; i++) {
        int start_idx = ia[i] - 1;  // Fortran转C索引
        int end_idx = ia[i+1] - 1;  // Fortran转C索引

        for (int j = start_idx; j < end_idx; j++) {
            int col = ja[j] - 1;  // Fortran转C索引
            full_matrix[i][col] = a[j];
            if (i != col) {  // 如果不是对角线元素，也填充对称位置
                full_matrix[col][i] = a[j];
            }
        }
    }

    // 计算 Ax
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Ax[i] += full_matrix[i][j] * x[j];
        }
    }

    print_vector("A*x", Ax.data(), n);

    // 计算残差
    for (int i = 0; i < n; ++i) {
        residual[i] = Ax[i] - b[i];
    }
    print_vector("残差 A*x - b", residual.data(), n);

    double max_residual = 0.0;
    for (int i = 0; i < n; ++i) {
        max_residual = max(max_residual, abs(residual[i]));
    }
    cout << "最大残差: " << max_residual << endl;
    if (max_residual < 1e-10) {
        cout << "验证通过：残差接近零，解正确！" << endl;
    } else {
        cout << "验证失败：残差较大，解可能不正确" << endl;
    }
    cout << endl;

    // 阶段4: 释放内存
    cout << "=== 阶段4: 释放内存 ===" << endl;
    phase = -1;  // 释放所有内存
    pardiso(pt, &maxfct, &mnum, &mtype, &phase, &n,
            a.data(), ia.data(), ja.data(),
            nullptr, &nrhs, iparm, &msglvl, nullptr, nullptr, &error);

    cout << "Pardiso求解完成" << endl << endl;

    // ========== 示例2: 非对称矩阵求解（基于官方非对称示例） ==========
    cout << "=== 示例2: 非对称矩阵求解（官方非对称示例风格） ===" << endl;

    // 使用官方非对称示例中的矩阵数据（5x5非对称矩阵）
    const int n2 = 5;
    vector<int> ia2 = {1, 4, 6, 9, 12, 14};  // Fortran风格索引
    vector<int> ja2 = {
        1, 2,    4,      // 第1行：列1, 2, 4
        1, 2,            // 第2行：列1, 2
              3, 4, 5,   // 第3行：列3, 4, 5
        1,    3, 4,      // 第4行：列1, 3, 4
           2,       5    // 第5行：列2, 5
    };
    vector<double> a2 = {
        1.0, -1.0,     -3.0,  // 第1行
        -2.0, 5.0,            // 第2行
                  4.0, 6.0, 4.0,  // 第3行
        -4.0,      2.0, 7.0,      // 第4行
              8.0,          -5.0  // 第5行
    };
    vector<double> b2(n2, 1.0);  // 右端向量 b = [1, 1, 1, 1, 1]
    vector<double> x2(n2, 0.0);

    print_matrix_csr("稀疏矩阵A", ia2.data(), ja2.data(), a2.data(), n2, ja2.size(), true);
    print_vector("右端向量b", b2.data(), n2);

    // 重新初始化Pardiso
    for (int i = 0; i < 64; ++i) {
        pt[i] = nullptr;
        iparm[i] = 0;
    }

    // 设置参数（非对称矩阵，基于官方非对称示例）
    iparm[0] = 1;         /* 不使用求解器默认值 */
    iparm[1] = 2;         /* 填充减少重排序来自METIS */
    iparm[3] = 0;         /* 无迭代-直接算法 */
    iparm[4] = 0;         /* 无用户填充减少排列 */
    iparm[5] = 0;         /* 解写入x */
    iparm[6] = 0;         /* 未使用 */
    iparm[7] = 2;         /* 最大迭代精化步骤数 */
    iparm[8] = 0;         /* 未使用 */
    iparm[9] = 13;        /* 用1E-13扰动主元元素 */
    iparm[10] = 1;        /* 使用非对称排列和缩放MPS */
    iparm[11] = 0;        /* 共轭转置/转置求解 */
    iparm[12] = 1;        /* 最大加权匹配算法被开启（非对称矩阵的默认值） */
    iparm[13] = 0;        /* 输出：扰动主元的数量 */
    iparm[14] = 0;        /* 未使用 */
    iparm[15] = 0;        /* 未使用 */
    iparm[16] = 0;        /* 未使用 */
    iparm[17] = -1;       /* 输出：LU因子中的非零元素数 */
    iparm[18] = -1;       /* 输出：LU分解的MFLOPS */
    iparm[19] = 0;        /* 输出：CG迭代次数 */
    iparm[26] = 0;        /* 0=无矩阵检查器，1=运行矩阵检查器 */
    iparm[27] = 0;        /* 0=双精度，1=单精度 */
    iparm[34] = 0;        /* 0=Fortran索引（1开始），1=C索引（0开始） */
    iparm[35] = 0;        /* 0=Fortran接口，1=C接口 */
    iparm[36] = 0;        /* 0=CSR格式，1=CSC格式 */
    iparm[59] = 0;        /* 0=核心内模式，1=核心内和核心外之间的自动切换 */

    int mtype2 = 11;  // 实非对称矩阵

    // 符号分解
    phase = 11;
    pardiso(pt, &maxfct, &mnum, &mtype2, &phase, &n2,
            a2.data(), ia2.data(), ja2.data(),
            nullptr, &nrhs, iparm, &msglvl, nullptr, nullptr, &error);

    if (error != 0) {
        cout << "符号分解错误，错误代码: " << error << endl;
        return 1;
    }

    cout << "符号分解完成..." << endl;
    cout << "LU中的非零元素数 = " << iparm[17] << endl;
    cout << "分解的MFLOPS = " << iparm[18] << endl;

    // 数值分解
    phase = 22;
    pardiso(pt, &maxfct, &mnum, &mtype2, &phase, &n2,
            a2.data(), ia2.data(), ja2.data(),
            nullptr, &nrhs, iparm, &msglvl, nullptr, nullptr, &error);

    if (error != 0) {
        cout << "数值分解错误，错误代码: " << error << endl;
        return 1;
    }

    cout << "分解完成..." << endl;

    // 求解
    phase = 33;
    pardiso(pt, &maxfct, &mnum, &mtype2, &phase, &n2,
            a2.data(), ia2.data(), ja2.data(),
            nullptr, &nrhs, iparm, &msglvl, b2.data(), x2.data(), &error);

    if (error != 0) {
        cout << "求解错误，错误代码: " << error << endl;
        return 1;
    }

    cout << "求解完成..." << endl;
    cout << "系统的解: " << endl;
    print_vector("x", x2.data(), n2);

    // 验证非对称矩阵解
    cout << "=== 验证解 ===" << endl;
    vector<double> Ax2(n2, 0.0);
    vector<double> residual2(n2, 0.0);

    // 计算 Ax（非对称矩阵）
    for (int i = 0; i < n2; i++) {
        int start_idx = ia2[i] - 1;  // Fortran转C索引
        int end_idx = ia2[i+1] - 1;  // Fortran转C索引

        for (int j = start_idx; j < end_idx; j++) {
            int col = ja2[j] - 1;  // Fortran转C索引
            Ax2[i] += a2[j] * x2[col];
        }
    }

    print_vector("A*x", Ax2.data(), n2);

    for (int i = 0; i < n2; ++i) {
        residual2[i] = Ax2[i] - b2[i];
    }
    print_vector("残差 A*x - b", residual2.data(), n2);

    double max_residual2 = 0.0;
    for (int i = 0; i < n2; ++i) {
        max_residual2 = max(max_residual2, abs(residual2[i]));
    }
    cout << "最大残差: " << max_residual2 << endl;
    if (max_residual2 < 1e-10) {
        cout << "验证通过：残差接近零，解正确！" << endl;
    } else {
        cout << "验证失败：残差较大，解可能不正确" << endl;
    }
    cout << endl;

    // 释放内存
    phase = -1;
    pardiso(pt, &maxfct, &mnum, &mtype2, &phase, &n2,
            a2.data(), ia2.data(), ja2.data(),
            nullptr, &nrhs, iparm, &msglvl, nullptr, nullptr, &error);

    cin.get();
    return 0;
}
