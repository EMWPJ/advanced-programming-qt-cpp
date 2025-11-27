#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

// Intel MKL 非线性求解器头文件
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

void print_vector(const char* name, const double* vec, int size, int max_print = 10) {
    cout << name << ":" << endl;
    int print_size = min(size, max_print);
    for (int i = 0; i < print_size; ++i) {
        cout << vec[i] << " ";
    }
    if (size > max_print) {
        cout << "... (共 " << size << " 个元素)";
    }
    cout << endl << endl;
}

// Newton法的简化实现（一维）
double newton_method_1d(double x0, int max_iter = 100, double tol = 1e-6) {
    // 求解 f(x) = x^2 - 4 = 0
    // f'(x) = 2*x

    double x = x0;

    for (int iter = 0; iter < max_iter; ++iter) {
        double f = x * x - 4.0;
        double df = 2.0 * x;

        if (abs(f) < tol) {
            cout << "Newton法收敛于迭代 " << iter << ", x = " << x << endl;
            return x;
        }

        if (abs(df) < 1e-12) {
            cout << "Newton法失败：导数为零" << endl;
            return x;
        }

        x = x - f / df;
    }

    cout << "Newton法达到最大迭代次数" << endl;
    return x;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL 非线性求解器示例 ===" << endl;
    cout << "包括: Newton法、拟Newton法、信赖域方法等" << endl << endl;

    // 示例1: Newton法（一维）
    cout << "\n=== 1. Newton法（一维） ===" << endl;
    {
        // 求解 f(x) = x^2 - 4 = 0
        double x0 = 3.0;
        cout << "求解方程: x^2 - 4 = 0" << endl;
        cout << "初始值: x0 = " << x0 << endl;

        double root = newton_method_1d(x0, 100, 1e-6);
        cout << "根: x = " << root << endl;
        cout << "验证: f(" << root << ") = " << root * root - 4.0 << endl << endl;
    }

    // 示例2: Newton法（多维系统）
    cout << "\n=== 2. Newton法（多维系统） ===" << endl;
    {
        const int n = 2;
        vector<double> x(n, 1.0);  // 初始值

        // 求解系统:
        // f1(x1, x2) = x1^2 + x2^2 - 5 = 0
        // f2(x1, x2) = x1 * x2 - 2 = 0

        cout << "求解非线性方程组:" << endl;
        cout << "  f1(x1, x2) = x1^2 + x2^2 - 5 = 0" << endl;
        cout << "  f2(x1, x2) = x1 * x2 - 2 = 0" << endl;
        print_vector("初始值x", x.data(), n, n);

        for (int iter = 0; iter < 20; ++iter) {
            // 计算函数值
            double f1 = x[0] * x[0] + x[1] * x[1] - 5.0;
            double f2 = x[0] * x[1] - 2.0;

            double norm_f = sqrt(f1 * f1 + f2 * f2);
            if (norm_f < 1e-6) {
                cout << "Newton法收敛于迭代 " << iter << endl;
                print_vector("解x", x.data(), n, n);
                cout << "验证: f1 = " << f1 << ", f2 = " << f2 << endl;
                break;
            }

            // 计算Jacobian矩阵
            // J = [2*x1, 2*x2]
            //     [x2,   x1]
            double J[4] = {2.0 * x[0], 2.0 * x[1], x[1], x[0]};

            // 求解线性系统: J * delta_x = -f
            double det = J[0] * J[3] - J[1] * J[2];
            if (abs(det) < 1e-12) {
                cout << "Newton法失败：Jacobian奇异" << endl;
                break;
            }

            double delta_x1 = (-J[3] * f1 + J[1] * f2) / det;
            double delta_x2 = (J[2] * f1 - J[0] * f2) / det;

            x[0] += delta_x1;
            x[1] += delta_x2;
        }

        cout << endl;
    }

    // 示例3: 拟Newton法（BFGS）概念
    cout << "\n=== 3. 拟Newton法（BFGS概念） ===" << endl;
    {
        cout << "BFGS方法通过近似Hessian矩阵来避免计算二阶导数" << endl;
        cout << "基本思想：使用一阶导数信息更新Hessian近似" << endl;
        cout << "更新公式: B_{k+1} = B_k + ..." << endl;
        cout << "注意：完整实现需要使用MKL的非线性求解器库" << endl << endl;
    }

    // 示例4: 信赖域方法概念
    cout << "\n=== 4. 信赖域方法概念 ===" << endl;
    {
        cout << "信赖域方法在每次迭代中限制步长" << endl;
        cout << "在信赖域内求解二次模型近似原问题" << endl;
        cout << "根据实际减少量与预测减少量的比值调整信赖域半径" << endl;
        cout << "注意：完整实现需要使用MKL的非线性求解器库" << endl << endl;
    }

    // 示例5: 非线性最小二乘
    cout << "\n=== 5. 非线性最小二乘 ===" << endl;
    {
        const int n = 5;  // 数据点数量
        const int m = 2;  // 参数数量

        vector<double> x_data = {0.0, 1.0, 2.0, 3.0, 4.0};
        vector<double> y_data = {1.0, 2.5, 5.0, 8.5, 13.0};

        // 拟合模型: y = a * x^2 + b
        vector<double> params(m, 1.0);  // 初始参数

        cout << "非线性最小二乘问题:" << endl;
        cout << "  模型: y = a * x^2 + b" << endl;
        print_vector("数据x", x_data.data(), n, n);
        print_vector("数据y", y_data.data(), n, n);

        // 计算残差
        vector<double> residuals(n);
        for (int i = 0; i < n; ++i) {
            residuals[i] = y_data[i] - (params[0] * x_data[i] * x_data[i] + params[1]);
        }

        double sum_squares = 0.0;
        for (int i = 0; i < n; ++i) {
            sum_squares += residuals[i] * residuals[i];
        }

        cout << "初始参数: a = " << params[0] << ", b = " << params[1] << endl;
        cout << "初始残差平方和: " << sum_squares << endl;
        cout << "注意：需要迭代优化参数，可使用Levenberg-Marquardt算法" << endl << endl;
    }

    // 示例6: Levenberg-Marquardt算法概念
    cout << "\n=== 6. Levenberg-Marquardt算法概念 ===" << endl;
    {
        cout << "Levenberg-Marquardt是专门用于非线性最小二乘问题的算法" << endl;
        cout << "结合了Gauss-Newton法和梯度下降法的优点" << endl;
        cout << "求解: min ||F(x)||^2" << endl;
        cout << "更新公式: (J^T * J + lambda * I) * delta_x = -J^T * F" << endl;
        cout << "其中lambda是阻尼参数，根据性能调整" << endl;
        cout << "注意：完整实现需要使用MKL的非线性求解器库" << endl << endl;
    }

    // 示例7: 全局优化概念
    cout << "\n=== 7. 全局优化概念 ===" << endl;
    {
        cout << "全局优化方法用于寻找全局最优解（而非局部最优）" << endl;
        cout << "常用方法包括:" << endl;
        cout << "  - 模拟退火" << endl;
        cout << "  - 遗传算法" << endl;
        cout << "  - 粒子群优化" << endl;
        cout << "  - 多起点方法" << endl;
        cout << "注意：MKL主要关注局部优化方法" << endl << endl;
    }

    cout << "\n注意：以上是非线性求解器的概念性示例。" << endl;
    cout << "实际应用中，MKL提供了更高效的非线性求解器接口，" << endl;
    cout << "包括完整的Newton法、拟Newton法、信赖域方法等，" << endl;
    cout << "建议使用MKL的完整非线性求解器库以获得更好的性能和稳定性。" << endl;

    cin.get();
    return 0;
}
