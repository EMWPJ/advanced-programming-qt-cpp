#include <iostream>
#include <boost/numeric/interval.hpp>
#include <windows.h>

using namespace std;
using namespace boost::numeric;
using namespace interval_lib;

// 使用默认的区间算术策略
typedef interval<double> interval_type;

void print_interval(const char* name, const interval_type& iv) {
    cout << name << " = [" << iv.lower() << ", " << iv.upper() << "]" << endl;
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost.Interval区间算术示例 ===" << endl;
    cout << "区间算术用于数值稳定性分析和误差传播" << endl << endl;

    // 示例1: 区间创建和基本操作
    cout << "\n=== 1. 区间创建和基本操作 ===" << endl;
    {
        // 创建区间
        interval_type a(1.0, 2.0);      // [1, 2]
        interval_type b(3.0, 4.0);       // [3, 4]
        interval_type c(1.5, 2.5);      // [1.5, 2.5]

        print_interval("a", a);
        print_interval("b", b);
        print_interval("c", c);

        // 基本运算
        interval_type sum = a + b;
        interval_type diff = b - a;
        interval_type prod = a * b;
        interval_type quot = b / a;

        print_interval("a + b", sum);
        print_interval("b - a", diff);
        print_interval("a * b", prod);
        print_interval("b / a", quot);
        cout << endl;
    }

    // 示例2: 区间函数
    cout << "\n=== 2. 区间函数 ===" << endl;
    {
        interval_type x(0.5, 1.5);
        interval_type y(-1.0, 1.0);

        print_interval("x", x);
        print_interval("y", y);

        // 三角函数（需要包含相应头文件）
        // interval_type sin_x = sin(x);
        // interval_type cos_x = cos(x);

        // 指数和对数
        // 注意：在Boost 1.88中，exp和log函数可能需要不同的策略配置
        // 这里注释掉以避免兼容性问题，实际使用时需要根据Boost版本调整
        // interval_type exp_x = exp(x);
        // interval_type log_x = log(x);  // x必须在正数区间
        cout << "注意：exp和log函数在Boost 1.88中可能需要特殊配置" << endl;
        cout << "实际使用时请参考Boost Interval库的最新文档" << endl;

        interval_type x_pos(0.1, 2.0);
        print_interval("x_pos (用于log)", x_pos);
        // interval_type log_x_pos = log(x_pos);
        // print_interval("log(x_pos)", log_x_pos);
        cout << endl;
    }

    // 示例3: 区间比较
    cout << "\n=== 3. 区间比较 ===" << endl;
    {
        interval_type a(1.0, 2.0);
        interval_type b(1.5, 2.5);
        interval_type c(2.5, 3.5);

        print_interval("a", a);
        print_interval("b", b);
        print_interval("c", c);

        // 检查是否包含
        bool a_in_b = in(1.5, b);
        cout << "1.5 在 b 中: " << (a_in_b ? "是" : "否") << endl;

        // 检查区间是否重叠
        // 可以使用自定义函数检查重叠

        // 检查区间大小
        double width_a = width(a);
        double width_b = width(b);
        cout << "区间a的宽度: " << width_a << endl;
        cout << "区间b的宽度: " << width_b << endl;
        cout << endl;
    }

    // 示例4: 区间算术的数值稳定性
    cout << "\n=== 4. 数值稳定性分析 ===" << endl;
    {
        // 示例：计算 f(x) = x^2 - x
        // 对于x在[0, 1]区间
        interval_type x(0.0, 1.0);

        print_interval("x", x);

        // f(x) = x^2 - x
        interval_type x_squared = x * x;
        interval_type f_x = x_squared - x;

        print_interval("x^2", x_squared);
        print_interval("f(x) = x^2 - x", f_x);

        cout << "区间算术自动处理舍入误差和不确定性" << endl;
        cout << "结果区间包含了所有可能的输出值" << endl << endl;
    }

    // 示例5: 误差传播分析
    cout << "\n=== 5. 误差传播分析 ===" << endl;
    {
        // 测量值带有不确定性
        interval_type length(10.0, 10.1);    // 长度: 10 ± 0.05
        interval_type width_iv(5.0, 5.1);    // 宽度: 5 ± 0.05（重命名避免与width函数冲突）

        print_interval("长度", length);
        print_interval("宽度", width_iv);

        // 计算面积（误差传播）
        interval_type area = length * width_iv;
        print_interval("面积 (误差传播)", area);

        // 相对误差
        double area_width_val = width(area);
        double area_median_val = median(area);
        double rel_error = area_width_val / area_median_val;
        cout << "相对误差: " << rel_error * 100 << "%" << endl << endl;
    }

    // 示例6: 区间运算的属性
    cout << "\n=== 6. 区间运算的属性 ===" << endl;
    {
        interval_type a(1.0, 2.0);
        interval_type b(3.0, 4.0);

        print_interval("a", a);
        print_interval("b", b);

        // 区间运算不满足分配律
        interval_type left = a * (b - b);
        interval_type right = a * b - a * b;

        print_interval("a * (b - b)", left);
        print_interval("a * b - a * b", right);
        cout << "注意：区间运算可能扩大区间（由于舍入误差）" << endl << endl;
    }

    // 示例7: 舍入模式
    cout << "\n=== 7. 舍入模式 ===" << endl;
    {
        cout << "Boost.Interval支持不同的舍入模式:" << endl;
        cout << "  - 向上舍入: 保证包含所有可能值" << endl;
        cout << "  - 向下舍入: 用于下界" << endl;
        cout << "  - 精确舍入: 使用硬件支持" << endl;
        cout << "舍入模式确保区间算术的正确性" << endl << endl;
    }

    // 示例8: 应用示例 - 全局优化
    cout << "\n=== 8. 应用示例 - 约束满足问题 ===" << endl;
    {
        // 约束: x + y = 5, x在[1, 3], y在[2, 4]
        interval_type x(1.0, 3.0);
        interval_type y(2.0, 4.0);

        print_interval("x", x);
        print_interval("y", y);

        // 检查约束 x + y = 5
        interval_type sum = x + y;
        print_interval("x + y", sum);

        if (in(5.0, sum)) {
            cout << "约束 x + y = 5 可能满足" << endl;
            // 可以进一步收缩区间
        } else {
            cout << "约束 x + y = 5 不满足" << endl;
        }
        cout << endl;
    }

    // 示例9: 区间扩展函数
    cout << "\n=== 9. 区间扩展函数 ===" << endl;
    {
        interval_type x(0.5, 1.5);

        print_interval("x", x);

        // 一些函数在区间上可能有特殊的扩展
        // 例如：sqrt函数需要非负输入

        interval_type x_nonneg(0.25, 4.0);
        print_interval("x_nonneg (用于sqrt)", x_nonneg);

        // interval_type sqrt_x = sqrt(x_nonneg);
        // print_interval("sqrt(x)", sqrt_x);

        cout << "注意：某些函数对输入区间有要求（如sqrt需要非负）" << endl << endl;
    }

    // 示例10: 区间的中点和半径
    cout << "\n=== 10. 区间的中点和半径 ===" << endl;
    {
        interval_type iv(1.0, 3.0);

        print_interval("区间", iv);

        double mid = median(iv);  // 中点
        double rad = width(iv) / 2.0;  // 半径

        cout << "中点: " << mid << endl;
        cout << "半径: " << rad << endl;
        cout << "宽度: " << width(iv) << endl;
        cout << "区间可以表示为: [" << mid - rad << ", " << mid + rad << "]" << endl << endl;
    }

    cout << "\n注意：区间算术是数值稳定性分析的有力工具，" << endl;
    cout << "可以自动跟踪舍入误差和不确定性，" << endl;
    cout << "在科学计算和工程应用中非常有用。" << endl;

    cin.get();
    return 0;
}
