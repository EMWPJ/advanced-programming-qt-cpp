#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <cmath>

// Intel MKL 变换和转置模块头文件
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_matrix(const char* name, const double* matrix, int rows, int cols, int max_print = 5) {
    cout << name << " (" << rows << "x" << cols << "):" << endl;
    int print_rows = min(rows, max_print);
    int print_cols = min(cols, max_print);
    for (int i = 0; i < print_rows; ++i) {
        for (int j = 0; j < print_cols; ++j) {
            cout << matrix[i * cols + j] << "\t";
        }
        if (cols > print_cols) cout << "...";
        cout << endl;
    }
    if (rows > print_rows) cout << "...\n";
    cout << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL Trans（变换和转置）模块示例 ===" << endl;
    cout << "包括: 矩阵转置、数据布局变换等" << endl << endl;

    // 示例1: 矩阵转置（密集矩阵）
    cout << "\n=== 1. 矩阵转置（密集矩阵） ===" << endl;
    {
        const int m = 4, n = 3;
        vector<double> A(m * n), AT(n * m);

        // 初始化矩阵A（行主序）
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = i * n + j + 1;
            }
        }

        print_matrix("原矩阵A", A.data(), m, n, 4);

        // 手动转置
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                AT[j * m + i] = A[i * n + j];
            }
        }

        print_matrix("转置矩阵A^T", AT.data(), n, m, 3);

        // 使用MKL的转置函数
        // mkl_domatcopy('C', 'T', m, n, 1.0, A, n, AT_mkl, m);
        cout << "注意：可以使用MKL的mkl_domatcopy进行高效转置" << endl << endl;
    }

    // 示例2: 使用MKL转置函数
    cout << "\n=== 2. 使用MKL转置函数 ===" << endl;
    {
        const int m = 3, n = 4;
        vector<double> A(m * n), AT(n * m);

        // 初始化矩阵A
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = i * n + j + 1;
            }
        }

        print_matrix("原矩阵A", A.data(), m, n, 3);

        // 使用MKL的mkl_domatcopy进行转置
        // 'C'表示C风格（行主序）
        // 'T'表示转置
        mkl_domatcopy('C', 'T', m, n, 1.0, A.data(), n, AT.data(), m);

        print_matrix("转置矩阵A^T (MKL)", AT.data(), n, m, 4);
    }

    // 示例3: 矩阵共轭转置（复数矩阵）
    cout << "\n=== 3. 矩阵共轭转置（复数矩阵） ===" << endl;
    {
        const int m = 2, n = 3;
        vector<complex<double>> A(m * n), AH(n * m);

        // 初始化复数矩阵
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = complex<double>(i * n + j + 1, (i * n + j) * 0.1);
            }
        }

        cout << "原矩阵A (复数):" << endl;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << "(" << A[i*n+j].real() << "," << A[i*n+j].imag() << ")\t";
            }
            cout << endl;
        }
        cout << endl;

        // 共轭转置: A^H = (A^*)^T
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                AH[j * m + i] = conj(A[i * n + j]);
            }
        }

        cout << "共轭转置矩阵A^H:" << endl;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << "(" << AH[i*m+j].real() << "," << AH[i*m+j].imag() << ")\t";
            }
            cout << endl;
        }
        cout << endl;

        // 使用MKL的复数转置函数
        // mkl_zomatcopy('C', 'C', m, n, 1.0, A, n, AH_mkl, m);
        // 'C'第二个表示共轭转置
        cout << "注意：可以使用MKL的mkl_zomatcopy进行高效共轭转置" << endl << endl;
    }

    // 示例4: 原地转置
    cout << "\n=== 4. 原地转置（方阵） ===" << endl;
    {
        const int n = 4;
        vector<double> A(n * n);

        // 初始化矩阵
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A[i * n + j] = i * n + j + 1;
            }
        }

        print_matrix("转置前", A.data(), n, n, n);

        // 原地转置（仅适用于方阵）
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                swap(A[i * n + j], A[j * n + i]);
            }
        }

        print_matrix("转置后", A.data(), n, n, n);

        cout << "注意：原地转置只适用于方阵" << endl;
        cout << "MKL提供了高效的原地转置函数" << endl << endl;
    }

    // 示例5: 批量转置
    cout << "\n=== 5. 批量转置 ===" << endl;
    {
        const int batch = 3;
        const int m = 3, n = 2;

        vector<double> A(batch * m * n);
        vector<double> AT(batch * n * m);

        // 初始化批量矩阵
        for (int b = 0; b < batch; ++b) {
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    A[b * m * n + i * n + j] = b * 100 + i * n + j + 1;
                }
            }
        }

        // 批量转置
        for (int b = 0; b < batch; ++b) {
            const double* A_b = A.data() + b * m * n;
            double* AT_b = AT.data() + b * n * m;

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    AT_b[j * m + i] = A_b[i * n + j];
                }
            }
        }

        cout << "批量转置完成 (batch = " << batch << ")" << endl;
        cout << "注意：MKL支持高效的批量转置操作" << endl << endl;
    }

    // 示例6: 数据布局变换
    cout << "\n=== 6. 数据布局变换 ===" << endl;
    {
        cout << "常见的数据布局变换:" << endl;
        cout << "  1. 行主序 ↔ 列主序" << endl;
        cout << "     - C/C++: 行主序" << endl;
        cout << "     - Fortran: 列主序" << endl;
        cout << "  2. 数据打包/解包" << endl;
        cout << "  3. 分块矩阵操作" << endl;
        cout << "  4. 数据重排" << endl;
        cout << "MKL提供了高效的布局变换函数" << endl << endl;
    }

    // 示例7: 转置的性能考虑
    cout << "\n=== 7. 转置的性能考虑 ===" << endl;
    {
        cout << "转置操作的性能优化:" << endl;
        cout << "  1. 使用MKL的优化函数" << endl;
        cout << "  2. 考虑缓存友好的算法" << endl;
        cout << "  3. 对于大规模矩阵，可以使用分块转置" << endl;
        cout << "  4. 对于稀疏矩阵，使用专门的稀疏转置" << endl;
        cout << "  5. 批量转置可以利用SIMD指令" << endl;
        cout << "MKL的转置函数针对Intel架构进行了高度优化" << endl << endl;
    }

    cout << "\n注意：以上是Trans模块的基本示例。" << endl;
    cout << "实际应用中，MKL提供了更多高级的变换功能，" << endl;
    cout << "包括高效的矩阵转置、数据布局变换等，" << endl;
    cout << "建议使用MKL的完整Trans库以获得更好的性能。" << endl;

    cin.get();
    return 0;
}
