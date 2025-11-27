#ifndef MT_INVERSION_CORE_H
#define MT_INVERSION_CORE_H

#include "mt_model.h"
#include "mt_frequency_generator.h"
#include "mt_forward_solver.h"
#include "mt_jacobian_calculator.h"
#include "mt_regularization.h"
#include "mt_optimizer.h"
#include <vector>
#include <string>

/**
 * MT一维反演核心计算类（重构后）
 * 作为协调器，使用各个模块化的组件完成反演任务
 */
class MTInversionCore {
public:
    // 常量定义（保持向后兼容）
    static constexpr double MU0 = MT::MU0;  // 真空磁导率
    static constexpr int DEFAULT_M = 40;      // 默认模型层数
    static constexpr int DEFAULT_NFREQ = 61; // 默认频率点数
    static constexpr double DEFAULT_EPSILON = 1e-5;  // 默认Jacobian扰动步长
    static constexpr int DEFAULT_MAX_ITER = 20;      // 默认最大迭代次数
    static constexpr double DEFAULT_TOL_DM = 1e-4;   // 默认模型更新范数容差
    static constexpr double DEFAULT_LAMBDA = 1.0;    // 默认正则化参数
    static constexpr double DEFAULT_FIRST_LAYER_THICKNESS = 10.0;  // 默认第一层厚度（米）
    static constexpr double DEFAULT_THICKNESS_GROWTH = 1.2;  // 默认厚度增长系数

    // 结构体：反演参数（保持向后兼容）
    using InversionParams = MT::InversionParams;
    
    // 结构体：反演结果（保持向后兼容）
    using InversionResult = MT::InversionResult;

    // 构造函数
    MTInversionCore();
    ~MTInversionCore();

    // 生成频率数组（保持向后兼容）
    void generateFrequencies(std::vector<double>& periods, std::vector<double>& omega, int n);

    // 1D有限差分法正演MT响应（保持向后兼容）
    void forwardFDM(const std::vector<double>& mLogRho,
                    const std::vector<double>& omega,
                    const std::vector<double>& layerThicknesses,
                    std::vector<double>& dataOut);

    // 计算Jacobian矩阵（保持向后兼容）
    void computeJacobian(const std::vector<double>& m,
                        const std::vector<double>& omega,
                        const std::vector<double>& dSyn,
                        const std::vector<double>& layerThicknesses,
                        double epsilon,
                        std::vector<std::vector<double>>& J);

    // 执行反演
    InversionResult invert(const InversionParams& params);

    // 生成随机模型（使用MKL随机数生成器，带高频滤波）
    void generateRandomModel(int M, double minRho, double maxRho, 
                            double filterCutoff, std::vector<double>& mLogRho);
    
    // 计算层厚度数组
    void computeLayerThicknesses(int M, double firstThickness, double growthFactor,
                                 std::vector<double>& thicknesses, std::vector<double>& depths);

    // 生成随机模型并正演计算响应
    void generateRandomModelAndForward(int M, int nFreq,
                                       double minRho, double maxRho,
                                       double firstThickness, double growthFactor,
                                       double filterCutoff,
                                       std::vector<double>& mLogRho,
                                       std::vector<double>& layerThicknesses,
                                       std::vector<double>& layerDepths,
                                       std::vector<double>& periods,
                                       std::vector<double>& omega,
                                       std::vector<double>& dataOut);

    // 获取进度回调函数类型
    typedef void (*ProgressCallback)(int iteration, double residual, double dmNorm, void* userData);

    // 设置进度回调函数
    void setProgressCallback(ProgressCallback callback, void* userData = nullptr);

    // 获取各个模块的指针（用于高级定制）
    MT::ForwardSolver* getForwardSolver() { return &m_forwardSolver; }
    MT::JacobianCalculator* getJacobianCalculator() { return &m_jacobianCalculator; }
    MT::Regularization* getRegularization() { return &m_regularization; }
    MT::Optimizer* getOptimizer() { return &m_optimizer; }
    MT::FrequencyGenerator* getFrequencyGenerator() { return &m_frequencyGenerator; }

private:
    // 生成高斯随机数（用于添加噪声）
    double gaussianRandom(double mean, double stddev);

    // 模块化组件
    MT::FrequencyGenerator m_frequencyGenerator;
    MT::ForwardSolver m_forwardSolver;
    MT::JacobianCalculator m_jacobianCalculator;
    MT::Regularization m_regularization;
    MT::Optimizer m_optimizer;

    // 进度回调
    ProgressCallback m_progressCallback;
    void* m_progressUserData;
};

#endif // MT_INVERSION_CORE_H
