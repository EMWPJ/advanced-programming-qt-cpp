#ifndef MT_MODEL_H
#define MT_MODEL_H

#include <vector>
#include <string>

/**
 * MT反演数据模型模块
 * 定义模型参数、观测数据、结果等数据结构
 */

namespace MT {

// 常量定义
constexpr double MU0 = 4e-7 * 3.14159265358979323846;  // 真空磁导率

/**
 * 反演参数结构
 */
struct InversionParams {
    int M = 40;                          // 模型层数
    int nFreq = 61;                      // 频率点数
    double epsilon = 1e-5;               // Jacobian扰动步长
    int maxIter = 20;                    // 最大迭代次数
    double tolDm = 1e-4;                 // 模型更新范数容差
    double lambda = 1.0;                 // 正则化参数
    double firstLayerThickness = 10.0;    // 第一层厚度（米）
    double thicknessGrowth = 1.2;        // 厚度增长系数
    
    // 可选：如果提供了观测数据，将使用这些数据而不是生成新的
    std::vector<double> dObs;             // 观测数据（如果为空，将使用默认模型生成）
    std::vector<double> mTrue;           // 真实模型（如果为空，将使用默认模型）
    std::vector<double> periods;         // 周期数组（如果为空，将自动生成）
    std::vector<double> omega;           // 角频率数组（如果为空，将自动生成）
    std::vector<double> layerThicknesses; // 层厚度数组（如果为空，将自动计算）
    std::vector<double> layerDepths;     // 层深度数组（如果为空，将自动计算）
};

/**
 * 反演结果结构
 */
struct InversionResult {
    bool success = false;                    // 是否成功
    int nIterations = 0;                     // 迭代次数
    std::vector<double> mTrue;               // 真实模型（log10(ρ)）
    std::vector<double> mInit;               // 初始模型（log10(ρ)）
    std::vector<double> mFinal;              // 最终反演结果（log10(ρ)）
    std::vector<double> layerThicknesses;   // 各层厚度（米）
    std::vector<double> layerDepths;         // 各层顶部深度（米），从地表0开始
    std::vector<double> periods;             // 周期数组
    std::vector<double> omega;               // 角频率数组
    std::vector<double> dObs;                // 观测数据
    std::vector<double> dSyn;                // 最终合成数据
    std::vector<double> residualHistory;     // 残差历史
    std::vector<double> dmNormHistory;       // 模型更新范数历史
    std::string errorMessage;                // 错误信息
};

/**
 * 模型参数结构
 */
struct ModelParams {
    std::vector<double> mLogRho;            // 模型参数（log10(ρ)）
    std::vector<double> layerThicknesses;   // 层厚度数组
    std::vector<double> layerDepths;        // 层深度数组
};

/**
 * 频率参数结构
 */
struct FrequencyParams {
    std::vector<double> periods;            // 周期数组
    std::vector<double> omega;              // 角频率数组
    int nFreq = 61;                         // 频率点数
    double T_min = 0.001;                   // 最小周期（秒）
    double T_max = 1000.0;                  // 最大周期（秒）
};

/**
 * 观测数据结构
 */
struct ObservationData {
    std::vector<double> data;               // 观测数据（log10(ρ_a)和相位）
    std::vector<double> dataStd;            // 数据标准差（可选）
    int nFreq = 0;                          // 频率点数
};

} // namespace MT

#endif // MT_MODEL_H

