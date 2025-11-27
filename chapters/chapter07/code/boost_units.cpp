#include <iostream>
#include <boost/units/systems/si.hpp>
#include <boost/units/systems/cgs.hpp>
#include <boost/units/systems/si/io.hpp>
#include <boost/units/io.hpp>
#include <windows.h>

using namespace std;
using namespace boost::units;
// 避免命名空间冲突，使用明确的命名空间限定
using boost::units::si::length;
// 注意：不能使用 using boost::units::si::time; 因为与标准库的 time() 函数冲突
using boost::units::si::mass;
using boost::units::si::velocity;
using boost::units::si::acceleration;
using boost::units::si::force;
using boost::units::si::energy;
using boost::units::si::power;
using boost::units::si::temperature;
using boost::units::si::volume;
using boost::units::si::mass_density;
using boost::units::si::area;
using boost::units::si::pressure;
using boost::units::si::meter;
using boost::units::si::second;
using boost::units::si::kilogram;
using boost::units::si::meter_per_second_squared;
using boost::units::si::meter_per_second;
using boost::units::si::kelvin;
using boost::units::si::cubic_meter;
using boost::units::si::square_meter;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Boost.Units单位系统示例 ===" << endl;

    // 示例1: 基本单位使用
    cout << "\n1. 基本单位使用:" << endl;

    // 长度
    quantity<length> distance = 100.0 * meter;
    cout << "距离: " << distance.value() << " m" << endl;

    // 时间（使用完全限定名避免与标准库 time() 函数冲突）
    quantity<boost::units::si::time> time_span = 10.0 * second;
    cout << "时间: " << time_span.value() << " s" << endl;

    // 速度 = 距离 / 时间
    auto speed_val = distance / time_span;
    quantity<velocity> speed = speed_val;
    cout << "速度: " << speed.value() << " m/s" << endl;

    // 加速度 = 速度 / 时间
    auto accel_val = speed / time_span;
    quantity<acceleration> accel = accel_val;
    cout << "加速度: " << accel.value() << " m/s²" << endl;

    // 示例2: 质量、力和能量
    cout << "\n2. 质量、力和能量:" << endl;

    quantity<mass> m = 5.0 * kilogram;
    quantity<acceleration> g = 9.81 * meter_per_second_squared;

    // 力 = 质量 × 加速度
    quantity<force> F = m * g;
    cout << "质量: " << m.value() << " kg" << endl;
    cout << "重力加速度: " << g.value() << " m/s²" << endl;
    cout << "力: " << F.value() << " N" << endl;

    // 能量 = 力 × 距离
    quantity<energy> E = F * distance;
    cout << "能量: " << E.value() << " J" << endl;

    // 功率 = 能量 / 时间
    auto P_val = E / time_span;
    quantity<power> P = P_val;
    cout << "功率: " << P.value() << " W" << endl;

    // 示例3: 单位转换
    cout << "\n3. 单位转换:" << endl;

    // SI单位
    quantity<si::length> d_si = 1.0 * si::meter;
    cout << "SI单位: " << d_si.value() << " m" << endl;

    // 转换为CGS单位（需要显式转换）
    quantity<cgs::length> d_cgs = quantity<cgs::length>(d_si);
    cout << "CGS单位: " << d_cgs.value() << " cm" << endl;

    // 提取数值
    double value_si = d_si.value();
    double value_cgs = d_cgs.value();
    cout << "数值 (SI): " << value_si << endl;
    cout << "数值 (CGS): " << value_cgs << endl;

    // 示例4: 温度单位
    cout << "\n4. 温度单位:" << endl;

    quantity<temperature> T1 = 273.15 * kelvin;
    quantity<temperature> T2 = 373.15 * kelvin;
    quantity<temperature> delta_T = T2 - T1;

    cout << "温度1: " << T1.value() << " K" << endl;
    cout << "温度2: " << T2.value() << " K" << endl;
    cout << "温差: " << delta_T.value() << " K" << endl;

    // 示例5: 复合单位
    cout << "\n5. 复合单位:" << endl;

    // 密度 = 质量 / 体积
    quantity<volume> V = 2.0 * cubic_meter;
    quantity<mass_density> rho = m / V;
    cout << "体积: " << V.value() << " m³" << endl;
    cout << "质量: " << m.value() << " kg" << endl;
    cout << "密度: " << rho.value() << " kg/m³" << endl;

    // 压力 = 力 / 面积
    quantity<area> A = 0.1 * square_meter;
    quantity<pressure> p = F / A;
    cout << "面积: " << A.value() << " m²" << endl;
    cout << "压力: " << p.value() << " Pa" << endl;

    // 示例6: 单位检查（编译时）
    cout << "\n6. 单位检查示例:" << endl;
    cout << "以下代码会在编译时报错（已注释）:" << endl;
    cout << "// quantity<length> wrong = speed;  // 错误！" << endl;
    cout << "// quantity<velocity> wrong2 = distance;  // 错误！" << endl;
    cout << "单位系统在编译时检查单位一致性，防止单位错误。" << endl;

    // 示例7: 自定义单位
    cout << "\n7. 单位运算:" << endl;

    // 速度的平方
    quantity<velocity> v = 10.0 * meter_per_second;
    auto v_squared = v * v;
    cout << "速度: " << v.value() << " m/s" << endl;
    cout << "速度的平方: " << v_squared.value() << " (m/s)²" << endl;

    // 动能 = 0.5 * m * v²
    quantity<energy> kinetic_energy = 0.5 * m * v * v;
    cout << "动能: " << kinetic_energy.value() << " J" << endl;

    // 示例8: 不同单位系统的混合使用
    cout << "\n8. 单位系统混合使用:" << endl;

    quantity<si::length> length_si = 1000.0 * si::meter;
    quantity<cgs::length> length_cgs = 100000.0 * cgs::centimeter;

    // 可以进行比较（需要转换为同一单位系统）
    quantity<si::length> length_cgs_si = quantity<si::length>(length_cgs);
    if (length_si == length_cgs_si) {
        cout << "长度相等: " << length_si.value() << " m = " << length_cgs.value() << " cm" << endl;
    }

    // 可以相加（需要转换为同一单位系统）
    auto total_length = length_si + length_cgs_si;
    cout << "总长度: " << total_length.value() << " m" << endl;

    cin.get();
    return 0;
}
