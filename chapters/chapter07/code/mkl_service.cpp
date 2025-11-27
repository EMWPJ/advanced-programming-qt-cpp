#include <iostream>
#include <vector>
#include <chrono>

// Intel MKL Service模块头文件
#include <mkl.h>
#include <mkl_service.h>
#include <windows.h>

using namespace std;
using namespace chrono;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Intel MKL Service模块示例 ===" << endl;
    cout << "包括: 服务函数、工具函数、环境配置等" << endl << endl;

    // 示例1: 获取MKL版本信息
    cout << "\n=== 1. MKL版本信息 ===" << endl;
    {
        MKLVersion mkl_version;
        mkl_get_version(&mkl_version);

        cout << "MKL版本: " << mkl_version.MajorVersion << "."
             << mkl_version.MinorVersion << "."
             << mkl_version.UpdateVersion << endl;
        cout << "产品状态: " << mkl_version.ProductStatus << endl;
        cout << "构建日期: " << mkl_version.Build << endl;
        cout << "处理器优化: " << mkl_version.Processor << endl << endl;
    }

    // 示例2: 设置线程数
    cout << "\n=== 2. 线程数设置 ===" << endl;
    {
        int max_threads = mkl_get_max_threads();
        cout << "当前最大线程数: " << max_threads << endl;

        // 设置线程数为4
        mkl_set_num_threads(4);
        int new_threads = mkl_get_max_threads();
        cout << "设置后最大线程数: " << new_threads << endl;

        // 恢复到默认值
        mkl_set_num_threads(max_threads);
        cout << "已恢复到默认线程数: " << mkl_get_max_threads() << endl << endl;
    }

    // 示例3: 动态线程设置
    cout << "\n=== 3. 动态线程设置 ===" << endl;
    {
        // 设置MKL使用动态线程数
        mkl_set_dynamic(1);
        cout << "动态线程设置: 启用" << endl;

        int max_threads = mkl_get_max_threads();
        cout << "最大线程数: " << max_threads << endl;

        // 禁用动态线程
        mkl_set_dynamic(0);
        cout << "动态线程设置: 禁用" << endl << endl;
    }

    // 示例4: 获取处理器信息
    cout << "\n=== 4. 处理器信息 ===" << endl;
    {
        // 获取CPU频率信息
        // 注意：MKL_Get_Cpu_Frequency 在某些版本中可能不可用或参数不同
        // 这里使用替代方法获取CPU信息
        cout << "CPU频率信息: 请使用系统工具查询" << endl;
        cout << "注意：MKL_Get_Cpu_Frequency在某些MKL版本中可能不可用" << endl;

        // 获取CPU缓存信息
        cout << "注意：可以使用mkl_get_cpu_clocks获取CPU时钟信息" << endl << endl;
    }

    // 示例5: 内存分配建议
    cout << "\n=== 5. 内存管理 ===" << endl;
    {
        cout << "MKL内存管理:" << endl;
        cout << "  - 可以使用mkl_malloc分配对齐内存" << endl;
        cout << "  - 可以使用mkl_free释放内存" << endl;
        cout << "  - 对齐内存可以提高性能" << endl;

        // 分配对齐内存示例
        const int n = 1000;
        double* aligned_ptr = (double*)mkl_malloc(n * sizeof(double), 64);
        if (aligned_ptr != nullptr) {
            cout << "成功分配对齐内存 (" << n << " 个double)" << endl;

            // 使用内存...
            for (int i = 0; i < n; ++i) {
                aligned_ptr[i] = i;
            }

            // 释放内存
            mkl_free(aligned_ptr);
            cout << "内存已释放" << endl;
        }
        cout << endl;
    }

    // 示例6: 环境变量查询
    cout << "\n=== 6. 环境变量和配置 ===" << endl;
    {
        cout << "MKL相关的环境变量:" << endl;
        cout << "  - MKL_NUM_THREADS: 线程数" << endl;
        cout << "  - MKL_DYNAMIC: 动态线程" << endl;
        cout << "  - MKL_INTERFACE_LAYER: 接口层" << endl;
        cout << "可以通过环境变量或程序内设置来控制MKL行为" << endl << endl;
    }

    // 示例7: 性能监控
    cout << "\n=== 7. 性能监控 ===" << endl;
    {
        cout << "MKL性能相关函数:" << endl;
        cout << "  - mkl_peak_mem_usage: 峰值内存使用" << endl;
        cout << "  - mkl_mem_stat: 内存统计" << endl;
        cout << "  - mkl_cbwr_set: 设置CPU绑定" << endl;
        cout << "可以使用这些函数监控和优化MKL性能" << endl << endl;
    }

    // 示例8: 错误处理
    cout << "\n=== 8. 错误处理 ===" << endl;
    {
        cout << "MKL错误处理:" << endl;
        cout << "  - 大多数MKL函数返回错误代码" << endl;
        cout << "  - 可以检查返回值判断是否成功" << endl;
        cout << "  - 使用mkl_get_max_threads等函数查询当前配置" << endl;
        cout << "  - 建议在使用前检查MKL函数返回值" << endl << endl;
    }

    // 示例9: CPU绑定设置
    cout << "\n=== 9. CPU绑定设置 ===" << endl;
    {
        // 设置CPU绑定（示例）
        cout << "CPU绑定可以控制线程在特定CPU核心上运行" << endl;
        cout << "可以使用mkl_cbwr_set设置CPU绑定模式" << endl;
        cout << "常见模式:" << endl;
        cout << "  - MKL_CBWR_ALL: 所有线程" << endl;
        cout << "  - MKL_CBWR_BOUND: 绑定线程" << endl;
        cout << "  - MKL_CBWR_COMPATIBLE: 兼容模式" << endl << endl;
    }

    // 示例10: 内存对齐的重要性
    cout << "\n=== 10. 内存对齐的重要性 ===" << endl;
    {
        const int n = 1000;

        // 未对齐内存
        double* unaligned = new double[n];
        cout << "未对齐内存地址: " << (void*)unaligned << endl;

        // 对齐内存（64字节对齐）
        double* aligned = (double*)mkl_malloc(n * sizeof(double), 64);
        if (aligned != nullptr) {
            cout << "对齐内存地址: " << (void*)aligned << endl;
            cout << "对齐检查: " << ((uintptr_t)aligned % 64 == 0 ? "是" : "否") << endl;

            // 对齐内存可以提高SIMD操作的性能
            mkl_free(aligned);
        }

        delete[] unaligned;
        cout << "注意：对齐内存可以提高向量化操作的性能" << endl << endl;
    }

    cout << "\n注意：Service模块提供了许多实用的工具函数，" << endl;
    cout << "可以帮助配置、监控和优化MKL的使用。" << endl;
    cout << "建议查阅MKL文档了解所有可用的服务函数。" << endl;

    cin.get();
    return 0;
}
