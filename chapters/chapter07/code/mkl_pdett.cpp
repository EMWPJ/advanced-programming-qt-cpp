#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

// Intel MKL PDE求解器头文件
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL PDE TT（时间相关PDE）求解器示例 ===" << endl;
    cout << "求解时间相关的偏微分方程" << endl << endl;

    // 示例1: 热方程（扩散方程）
    cout << "\n=== 1. 热方程（一维） ===" << endl;
    {
        // 方程: ∂u/∂t = α * ∂²u/∂x²
        // 初始条件: u(x,0) = f(x)
        // 边界条件: u(0,t) = u(L,t) = 0

        const int nx = 10;  // 空间网格点数
        const int nt = 100;  // 时间步数
        const double L = 1.0;  // 域长度
        const double T = 1.0;  // 总时间
        const double alpha = 0.1;  // 扩散系数
        const double h = L / (nx - 1);  // 空间步长
        const double dt = T / nt;  // 时间步长

        cout << "热方程: ∂u/∂t = α * ∂²u/∂x²" << endl;
        cout << "空间网格: " << nx << " 点, 步长 h = " << h << endl;
        cout << "时间步数: " << nt << ", 步长 dt = " << dt << endl;
        cout << "扩散系数: α = " << alpha << endl;

        // 稳定性条件检查
        double r = alpha * dt / (h * h);
        cout << "稳定性参数: r = α*dt/h² = " << r << endl;
        if (r > 0.5) {
            cout << "警告: r > 0.5，显式格式可能不稳定！" << endl;
        }

        // 显式格式（FTCS）
        vector<double> u_old(nx, 0.0);
        vector<double> u_new(nx, 0.0);

        // 初始条件: u(x,0) = sin(π*x/L)
        for (int i = 0; i < nx; ++i) {
            double x = i * h;
            u_old[i] = sin(M_PI * x / L);
        }

        cout << "使用显式格式（Forward-Time Central-Space）" << endl;
        cout << "注意：实际求解需要进行时间步进" << endl << endl;
    }

    // 示例2: 波动方程
    cout << "\n=== 2. 波动方程（一维） ===" << endl;
    {
        // 方程: ∂²u/∂t² = c² * ∂²u/∂x²
        // 初始条件: u(x,0) = f(x), ∂u/∂t(x,0) = g(x)
        // 边界条件: u(0,t) = u(L,t) = 0

        const int nx = 10;
        const int nt = 100;
        const double L = 1.0;
        const double T = 1.0;
        const double c = 1.0;  // 波速
        const double h = L / (nx - 1);
        const double dt = T / nt;

        cout << "波动方程: ∂²u/∂t² = c² * ∂²u/∂x²" << endl;
        cout << "波速: c = " << c << endl;
        cout << "空间步长: h = " << h << endl;
        cout << "时间步长: dt = " << dt << endl;

        // CFL条件: c*dt/h <= 1
        double cfl = c * dt / h;
        cout << "CFL数: c*dt/h = " << cfl << endl;
        if (cfl > 1.0) {
            cout << "警告: CFL数 > 1，可能不稳定！" << endl;
        }

        // 需要三个时间层的值: u^{n-1}, u^n, u^{n+1}
        vector<double> u_prev(nx, 0.0);
        vector<double> u_curr(nx, 0.0);
        vector<double> u_next(nx, 0.0);

        cout << "使用显式格式，需要两个初始条件" << endl;
        cout << "注意：实际求解需要进行时间步进" << endl << endl;
    }

    // 示例3: 对流-扩散方程
    cout << "\n=== 3. 对流-扩散方程 ===" << endl;
    {
        // 方程: ∂u/∂t + v * ∂u/∂x = α * ∂²u/∂x²
        // 包含对流项和扩散项

        const int nx = 10;
        const double v = 0.5;  // 对流速度
        const double alpha = 0.1;  // 扩散系数

        cout << "对流-扩散方程: ∂u/∂t + v*∂u/∂x = α*∂²u/∂x²" << endl;
        cout << "对流速度: v = " << v << endl;
        cout << "扩散系数: α = " << alpha << endl;

        // Peclet数: Pe = v*L/α
        double L = 1.0;
        double pe = v * L / alpha;
        cout << "Peclet数: Pe = v*L/α = " << pe << endl;
        if (pe > 1.0) {
            cout << "对流占主导，需要使用特殊的数值格式（如upwind scheme）" << endl;
        }

        cout << "注意：需要平衡对流项和扩散项的离散化" << endl << endl;
    }

    // 示例4: 时间步进方法
    cout << "\n=== 4. 时间步进方法 ===" << endl;
    {
        cout << "常见的时间离散化方法:" << endl;
        cout << "  1. 显式方法（Euler前向）:" << endl;
        cout << "     - 简单，但可能不稳定" << endl;
        cout << "     - 需要小时间步长" << endl;
        cout << "  2. 隐式方法（Euler后向）:" << endl;
        cout << "     - 无条件稳定" << endl;
        cout << "     - 需要求解线性系统" << endl;
        cout << "  3. Crank-Nicolson方法:" << endl;
        cout << "     - 二阶精度" << endl;
        cout << "     - 无条件稳定" << endl;
        cout << "     - 需要求解线性系统" << endl;
        cout << "  4. Runge-Kutta方法:" << endl;
        cout << "     - 高精度" << endl;
        cout << "     - 多个时间层" << endl << endl;
    }

    // 示例5: 空间离散化方法
    cout << "\n=== 5. 空间离散化方法 ===" << endl;
    {
        cout << "常用的空间离散化方法:" << endl;
        cout << "  1. 有限差分法 (FDM):" << endl;
        cout << "     - 简单直接" << endl;
        cout << "     - 适用于规则网格" << endl;
        cout << "  2. 有限元法 (FEM):" << endl;
        cout << "     - 适用于复杂几何" << endl;
        cout << "     - 变分形式" << endl;
        cout << "  3. 有限体积法 (FVM):" << endl;
        cout << "     - 守恒性好" << endl;
        cout << "     - 适用于守恒律" << endl;
        cout << "  4. 谱方法:" << endl;
        cout << "     - 高精度" << endl;
        cout << "     - 光滑解" << endl << endl;
    }

    // 示例6: 稳定性分析
    cout << "\n=== 6. 数值稳定性分析 ===" << endl;
    {
        cout << "稳定性是时间相关PDE求解的关键" << endl;
        cout << "常见稳定性条件:" << endl;
        cout << "  1. 扩散方程显式格式: r = α*dt/h² < 0.5" << endl;
        cout << "  2. 波动方程显式格式: CFL = c*dt/h < 1" << endl;
        cout << "  3. 对流方程: CFL = v*dt/h < 1" << endl;
        cout << "不满足稳定性条件可能导致数值解发散" << endl;
        cout << "隐式方法通常无条件稳定，但计算成本更高" << endl << endl;
    }

    // 示例7: 初始条件和边界条件
    cout << "\n=== 7. 初始条件和边界条件 ===" << endl;
    {
        cout << "时间相关PDE需要:" << endl;
        cout << "  1. 初始条件: u(x,0) = f(x)" << endl;
        cout << "     对于二阶时间导数还需要: ∂u/∂t(x,0) = g(x)" << endl;
        cout << "  2. 边界条件:" << endl;
        cout << "     - Dirichlet: u(边界,t) = h(t)" << endl;
        cout << "     - Neumann: ∂u/∂n(边界,t) = k(t)" << endl;
        cout << "     - Robin: α*u + β*∂u/∂n = l(t)" << endl;
        cout << "初始条件和边界条件的正确设置对解的准确性很重要" << endl << endl;
    }

    cout << "\n注意：以上是时间相关PDE求解器的概念性示例。" << endl;
    cout << "实际应用中，MKL提供了专门的PDE TT求解器接口，" << endl;
    cout << "可以高效处理各种时间相关的偏微分方程，" << endl;
    cout << "包括热方程、波动方程、对流-扩散方程等，" << endl;
    cout << "建议使用MKL的完整PDE TT求解器库以获得更好的性能和精度。" << endl;

    cin.get();
    return 0;
}
