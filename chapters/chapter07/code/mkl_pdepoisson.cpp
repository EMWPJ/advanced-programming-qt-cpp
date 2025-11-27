#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

// Intel MKL PDE求解器头文件
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
    cout << "=== Intel MKL PDE Poisson求解器示例 ===" << endl;
    cout << "求解泊松方程: -∇²u = f" << endl << endl;

    // 示例1: 一维Poisson方程
    cout << "\n=== 1. 一维Poisson方程 ===" << endl;
    {
        const int n = 10;  // 网格点数
        const double L = 1.0;  // 域长度
        const double h = L / (n - 1);  // 网格间距

        // 离散化: -u''(x) = f(x)
        // 有限差分: -(u_{i+1} - 2*u_i + u_{i-1}) / h² = f_i

        vector<double> u(n, 0.0);  // 解
        vector<double> f(n, 1.0);  // 右端项（常数）

        // 边界条件: u(0) = 0, u(L) = 0
        u[0] = 0.0;
        u[n-1] = 0.0;

        // 构建三对角矩阵
        // A * u = f
        // A是(n-2)x(n-2)的三对角矩阵

        const int m = n - 2;
        vector<int> row_ptr(m + 1);
        vector<int> col_idx(m * 3);
        vector<double> values(m * 3);

        int idx = 0;
        row_ptr[0] = 0;
        for (int i = 0; i < m; ++i) {
            int count = 0;
            if (i > 0) {
                col_idx[idx] = i - 1;
                values[idx] = -1.0 / (h * h);
                idx++;
                count++;
            }

            col_idx[idx] = i;
            values[idx] = 2.0 / (h * h);
            idx++;
            count++;

            if (i < m - 1) {
                col_idx[idx] = i + 1;
                values[idx] = -1.0 / (h * h);
                idx++;
                count++;
            }

            row_ptr[i + 1] = row_ptr[i] + count;
        }

        // 调整右端项（考虑边界条件）
        vector<double> b(m);
        for (int i = 0; i < m; ++i) {
            b[i] = f[i + 1];
            if (i == 0) b[i] += u[0] / (h * h);
            if (i == m - 1) b[i] += u[n-1] / (h * h);
        }

        cout << "一维Poisson方程离散化完成" << endl;
        cout << "网格点数: " << n << ", 网格间距: h = " << h << endl;
        cout << "内部节点数: " << m << endl;
        cout << "注意：实际求解需要使用MKL的线性求解器" << endl << endl;
    }

    // 示例2: 二维Poisson方程
    cout << "\n=== 2. 二维Poisson方程 ===" << endl;
    {
        const int nx = 5, ny = 5;  // x和y方向的网格点数
        const double Lx = 1.0, Ly = 1.0;  // 域大小
        const double hx = Lx / (nx - 1), hy = Ly / (ny - 1);

        // 离散化: -∇²u = -(u_xx + u_yy) = f
        // 五点差分格式:
        // -(u_{i+1,j} - 2*u_{i,j} + u_{i-1,j})/hx²
        // -(u_{i,j+1} - 2*u_{i,j} + u_{i,j-1})/hy² = f_{i,j}

        cout << "二维Poisson方程离散化" << endl;
        cout << "网格: " << nx << " x " << ny << endl;
        cout << "网格间距: hx = " << hx << ", hy = " << hy << endl;

        // 将2D网格展平为1D向量
        const int n_total = nx * ny;
        const int n_interior = (nx - 2) * (ny - 2);  // 内部节点数

        cout << "总节点数: " << n_total << ", 内部节点数: " << n_interior << endl;
        cout << "将生成 " << n_interior << "x" << n_interior << " 的稀疏矩阵" << endl;
        cout << "注意：实际求解需要使用MKL的稀疏线性求解器（如Pardiso）" << endl << endl;
    }

    // 示例3: 泊松方程的解析解
    cout << "\n=== 3. 泊松方程的解析解（示例） ===" << endl;
    {
        // 一维Poisson方程: -u''(x) = 1, u(0) = u(1) = 0
        // 解析解: u(x) = x*(1-x)/2

        const int n = 10;
        const double L = 1.0;
        const double h = L / (n - 1);

        cout << "一维Poisson方程: -u''(x) = 1, u(0) = u(1) = 0" << endl;
        cout << "解析解: u(x) = x*(1-x)/2" << endl;
        cout << "数值解 vs 解析解:" << endl;

        for (int i = 0; i < n; ++i) {
            double x = i * h;
            double u_exact = x * (1.0 - x) / 2.0;
            cout << "x = " << x << ", u_exact = " << u_exact << endl;
        }
        cout << endl;
    }

    // 示例4: 有限差分方法概念
    cout << "\n=== 4. 有限差分方法概念 ===" << endl;
    {
        cout << "有限差分方法将偏微分方程离散化为代数方程组" << endl;
        cout << "关键步骤:" << endl;
        cout << "  1. 网格划分" << endl;
        cout << "  2. 微分算子离散化" << endl;
        cout << "  3. 边界条件处理" << endl;
        cout << "  4. 构建线性系统" << endl;
        cout << "  5. 求解线性系统" << endl;
        cout << "Poisson方程离散化后通常得到对称正定稀疏矩阵" << endl;
        cout << "可以使用共轭梯度法或直接求解器（如Pardiso）" << endl << endl;
    }

    // 示例5: 边界条件类型
    cout << "\n=== 5. 边界条件类型 ===" << endl;
    {
        cout << "常见的边界条件:" << endl;
        cout << "  1. Dirichlet条件: u(x) = g(x) 在边界上" << endl;
        cout << "  2. Neumann条件: ∂u/∂n = h(x) 在边界上" << endl;
        cout << "  3. Robin条件: α*u + β*∂u/∂n = k(x) 在边界上" << endl;
        cout << "不同边界条件需要不同的处理方法" << endl;
        cout << "Dirichlet条件最简单，直接将边界值代入" << endl;
        cout << "Neumann条件需要在边界上添加额外的方程" << endl << endl;
    }

    cout << "\n注意：以上是PDE Poisson求解器的概念性示例。" << endl;
    cout << "实际应用中，MKL提供了专门的PDE求解器接口，" << endl;
    cout << "可以高效处理二维和三维Poisson方程，" << endl;
    cout << "包括各种边界条件和求解方法，" << endl;
    cout << "建议使用MKL的完整PDE求解器库以获得更好的性能。" << endl;

    cin.get();
    return 0;
}
