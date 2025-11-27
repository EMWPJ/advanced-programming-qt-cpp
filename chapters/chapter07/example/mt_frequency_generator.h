#ifndef MT_FREQUENCY_GENERATOR_H
#define MT_FREQUENCY_GENERATOR_H

#include "mt_model.h"
#include <vector>

/**
 * MT频率生成器模块
 * 负责生成MT反演所需的频率数组
 */
namespace MT {

class FrequencyGenerator {
public:
    FrequencyGenerator();
    ~FrequencyGenerator();

    /**
     * 生成频率数组（对数均匀分布）
     * @param nFreq 频率点数
     * @param T_min 最小周期（秒）
     * @param T_max 最大周期（秒）
     * @param periods 输出的周期数组
     * @param omega 输出的角频率数组
     */
    void generate(int nFreq, double T_min, double T_max,
                  std::vector<double>& periods, std::vector<double>& omega);

    /**
     * 生成频率数组（使用默认范围）
     * @param nFreq 频率点数
     * @param periods 输出的周期数组
     * @param omega 输出的角频率数组
     */
    void generate(int nFreq, std::vector<double>& periods, std::vector<double>& omega);

    /**
     * 生成频率参数结构
     * @param nFreq 频率点数
     * @param T_min 最小周期（秒）
     * @param T_max 最大周期（秒）
     * @return FrequencyParams结构
     */
    FrequencyParams generateParams(int nFreq, double T_min = 0.001, double T_max = 1000.0);

private:
    static constexpr double DEFAULT_T_MIN = 0.001;   // 默认最小周期（秒）
    static constexpr double DEFAULT_T_MAX = 1000.0;  // 默认最大周期（秒）
};

} // namespace MT

#endif // MT_FREQUENCY_GENERATOR_H

