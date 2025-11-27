#include "mt_inversion_core.h"
#include <mkl_vsl.h>
#include <mkl_vml.h>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <ctime>
#include <exception>
#include <stdexcept>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MTInversionCore::MTInversionCore()
    : m_jacobianCalculator(&m_forwardSolver)
    , m_progressCallback(nullptr)
    , m_progressUserData(nullptr) {
}

MTInversionCore::~MTInversionCore() {
}

void MTInversionCore::generateFrequencies(std::vector<double>& periods,
                                          std::vector<double>& omega,
                                          int n) {
    m_frequencyGenerator.generate(n, periods, omega);
}

void MTInversionCore::forwardFDM(const std::vector<double>& mLogRho,
                                const std::vector<double>& omega,
                                const std::vector<double>& layerThicknesses,
                                std::vector<double>& dataOut) {
    m_forwardSolver.solve(mLogRho, omega, layerThicknesses, dataOut);
}

void MTInversionCore::computeJacobian(const std::vector<double>& m,
                                     const std::vector<double>& omega,
                                     const std::vector<double>& dSyn,
                                     const std::vector<double>& layerThicknesses,
                                     double epsilon,
                                     std::vector<std::vector<double>>& J) {
    m_jacobianCalculator.compute(m, omega, dSyn, layerThicknesses, epsilon, J);
}

MTInversionCore::InversionResult MTInversionCore::invert(const InversionParams& params) {
    InversionResult result;

    try {
        int M = params.M;
        int nFreq = params.nFreq;
        int nData = nFreq * 2;

        // 1. 生成或使用提供的频率数组
        if (!params.periods.empty() && !params.omega.empty() && 
            params.periods.size() == nFreq && params.omega.size() == nFreq) {
            result.periods = params.periods;
            result.omega = params.omega;
        } else {
            m_frequencyGenerator.generate(nFreq, result.periods, result.omega);
        }

        // 2. 计算或使用提供的层厚度
        if (!params.layerThicknesses.empty() && !params.layerDepths.empty() &&
            params.layerThicknesses.size() == M && params.layerDepths.size() == M) {
            result.layerThicknesses = params.layerThicknesses;
            result.layerDepths = params.layerDepths;
        } else {
            computeLayerThicknesses(M, params.firstLayerThickness, params.thicknessGrowth,
                                    result.layerThicknesses, result.layerDepths);
        }

        // 3. 设置真实模型：如果提供了观测数据，使用提供的真实模型；否则使用默认模型
        if (!params.dObs.empty() && params.dObs.size() == nData && 
            !params.mTrue.empty() && params.mTrue.size() == M) {
            // 使用提供的观测数据和真实模型
            result.mTrue = params.mTrue;
            result.dObs = params.dObs;
            // 验证频率数组和层厚度数组是否匹配
            if (params.periods.size() != nFreq || params.omega.size() != nFreq) {
                throw std::runtime_error("提供的频率数组大小与nFreq不匹配");
            }
            if (params.layerThicknesses.size() != M || params.layerDepths.size() != M) {
                throw std::runtime_error("提供的层厚度数组大小与M不匹配");
            }
        } else {
            // 使用默认的真实模型
            result.mTrue.resize(M);
            int nLayers1 = std::min(5, M / 4);
            int nLayers2 = std::min(10, M / 2);
            
            for (int i = 0; i < nLayers1; i++) {
                result.mTrue[i] = log10(100.0);  // 前几层：100 Ω·m
            }
            for (int i = nLayers1; i < nLayers1 + nLayers2 && i < M; i++) {
                result.mTrue[i] = log10(10.0);   // 中间层：10 Ω·m
            }
            for (int i = nLayers1 + nLayers2; i < M; i++) {
                result.mTrue[i] = log10(1000.0);  // 最后层：1000 Ω·m
            }

            // 4. 生成合成观测数据（加2%高斯噪声）
            m_forwardSolver.solve(result.mTrue, result.omega, result.layerThicknesses, result.dObs);
            srand(12345);
            for (int i = 0; i < nData; i++) {
                double noiseLevel = 0.02;  // 2%
                double noise = gaussianRandom(0.0, noiseLevel * fabs(result.dObs[i]));
                result.dObs[i] += noise;
            }
        }

        // 5. 设置初始模型
        result.mInit.resize(M);
        std::vector<double> mCurrent(M);
        for (int i = 0; i < M; i++) {
            result.mInit[i] = log10(100.0);  // 均匀模型：100 Ω·m
            mCurrent[i] = result.mInit[i];
        }

        // 6. 构建正则化矩阵L和L^T*L
        std::vector<std::vector<double>> L;
        m_regularization.buildLMatrix(M, L);
        std::vector<std::vector<double>> LTL;
        m_regularization.computeLTL(L, LTL);

        // 7. 反演循环
        result.residualHistory.clear();
        result.dmNormHistory.clear();

        for (int iter = 0; iter < params.maxIter; iter++) {
            // 7.1 正演计算合成数据
            std::vector<double> dSyn;
            m_forwardSolver.solve(mCurrent, result.omega, result.layerThicknesses, dSyn);

            // 7.2 计算残差
            std::vector<double> r(nData);
            // 使用VML的vdSub计算向量差
            vdSub(nData, result.dObs.data(), dSyn.data(), r.data());
            // 检查NaN和Inf
            for (int i = 0; i < nData; i++) {
                if (!std::isfinite(r[i])) {
                    r[i] = 0.0;
                }
            }
            // 使用cblas_dnrm2计算范数
            double residualNorm = cblas_dnrm2(nData, r.data(), 1);
            // 检查结果
            if (!std::isfinite(residualNorm)) {
                residualNorm = 0.0;
            }
            result.residualHistory.push_back(residualNorm);

            // 7.3 计算Jacobian矩阵
            std::vector<std::vector<double>> J;
            m_jacobianCalculator.compute(mCurrent, result.omega, dSyn, 
                                        result.layerThicknesses, params.epsilon, J);

            // 7.4 计算J^T*J和J^T*r
            std::vector<std::vector<double>> JTJ;
            m_optimizer.computeJTJ(J, JTJ);
            std::vector<double> JTr;
            m_optimizer.computeJTr(J, r, JTr);

            // 7.5 求解正规方程
            std::vector<double> dm;
            bool success = m_optimizer.solve(JTJ, LTL, params.lambda, JTr, dm);
            if (!success) {
                result.errorMessage = "优化求解器失败";
                break;
            }

            // 7.6 计算模型更新范数
            // 使用cblas_dnrm2计算dm的范数
            for (int i = 0; i < M; i++) {
                // 检查NaN和Inf
                if (!std::isfinite(dm[i])) {
                    dm[i] = 0.0;
                }
            }
            double dmNorm = cblas_dnrm2(M, dm.data(), 1);
            // 检查结果
            if (!std::isfinite(dmNorm)) {
                dmNorm = 0.0;
            }
            result.dmNormHistory.push_back(dmNorm);

            // 调用进度回调
            if (m_progressCallback) {
                m_progressCallback(iter + 1, residualNorm, dmNorm, m_progressUserData);
            }

            // 7.7 更新模型
            for (int i = 0; i < M; i++) {
                mCurrent[i] += dm[i];
                // 检查更新后的值是否有效
                if (!std::isfinite(mCurrent[i])) {
                    mCurrent[i] = result.mInit[i];  // 如果无效，恢复初始值
                }
            }

            // 检查收敛条件
            if (dmNorm < params.tolDm) {
                result.nIterations = iter + 1;
                break;
            }

            result.nIterations = iter + 1;
        }

        // 8. 保存最终结果
        result.mFinal = mCurrent;
        m_forwardSolver.solve(result.mFinal, result.omega, result.layerThicknesses, result.dSyn);
        result.success = true;

    } catch (const std::exception& e) {
        result.errorMessage = std::string("异常: ") + e.what();
        result.success = false;
    }

    return result;
}

void MTInversionCore::setProgressCallback(ProgressCallback callback, void* userData) {
    m_progressCallback = callback;
    m_progressUserData = userData;
}

void MTInversionCore::computeLayerThicknesses(int M, double firstThickness, double growthFactor,
                                               std::vector<double>& thicknesses, std::vector<double>& depths) {
    thicknesses.resize(M);
    depths.resize(M);
    
    double currentThickness = firstThickness;
    double currentDepth = 0.0;
    
    for (int i = 0; i < M; i++) {
        thicknesses[i] = currentThickness;
        depths[i] = currentDepth;
        currentDepth += currentThickness;
        currentThickness *= growthFactor;  // 下一层厚度增长
    }
}

void MTInversionCore::generateRandomModel(int M, double minRho, double maxRho, 
                                         double filterCutoff, std::vector<double>& mLogRho) {
    mLogRho.resize(M);

    // 使用MKL VSL随机数生成器生成均匀分布的随机数
    VSLStreamStatePtr stream;
    int errcode = vslNewStream(&stream, VSL_BRNG_MT19937, static_cast<unsigned int>(time(nullptr)));

    std::vector<double> uniform(M);
    if (errcode == VSL_STATUS_OK) {
        // 生成[0,1]区间的均匀随机数
        vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, M, uniform.data(), 0.0, 1.0);
        vslDeleteStream(&stream);
    } else {
        // 如果MKL VSL失败，使用标准库随机数生成器作为后备
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < M; i++) {
            uniform[i] = static_cast<double>(rand()) / RAND_MAX;
        }
    }

    // 转换为对数空间的电阻率值
    double logMinRho = log10(minRho);
    double logMaxRho = log10(maxRho);
    std::vector<double> rawLogRho(M);
    for (int i = 0; i < M; i++) {
        rawLogRho[i] = logMinRho + uniform[i] * (logMaxRho - logMinRho);
    }

    // 应用高频滤波（改进的高斯加权移动平均滤波，使模型更光滑）
    if (filterCutoff > 0.0 && filterCutoff < 1.0) {
        // 使用更大的滤波窗口，使模型更光滑
        int windowSize = static_cast<int>(M * filterCutoff * 2.0);  // 增大窗口
        if (windowSize < 3) windowSize = 3;  // 最小窗口为3
        if (windowSize > M) windowSize = M;
        
        // 使用高斯加权而不是简单平均
        std::vector<double> filtered(M);
        double sigma = windowSize / 3.0;  // 高斯标准差
        
        for (int i = 0; i < M; i++) {
            double sum = 0.0;
            double weightSum = 0.0;
            int halfWindow = windowSize / 2;
            
            for (int j = -halfWindow; j <= halfWindow; j++) {
                int idx = i + j;
                if (idx >= 0 && idx < M) {
                    // 高斯权重
                    double weight = exp(-(j * j) / (2.0 * sigma * sigma));
                    sum += rawLogRho[idx] * weight;
                    weightSum += weight;
                }
            }
            
            if (weightSum > 1e-10) {
                filtered[i] = sum / weightSum;
            } else {
                filtered[i] = rawLogRho[i];
            }
        }
        
        // 应用第二次滤波以进一步平滑（可选）
        std::vector<double> smoothed(M);
        for (int i = 0; i < M; i++) {
            double sum = 0.0;
            double weightSum = 0.0;
            int halfWindow = windowSize / 4;  // 第二次使用较小的窗口
            
            for (int j = -halfWindow; j <= halfWindow; j++) {
                int idx = i + j;
                if (idx >= 0 && idx < M) {
                    // 使用VML的vdexp向量化计算权重
                    double arg = -(j * j) / (2.0 * sigma * sigma);
                    double weight;
                    int n_one = 1;
                    vdExp(n_one, &arg, &weight);
                    sum += filtered[idx] * weight;
                    weightSum += weight;
                }
            }
            
            if (weightSum > 1e-10) {
                smoothed[i] = sum / weightSum;
            } else {
                smoothed[i] = filtered[i];
            }
        }
        
        // 适度调整滤波后的数据范围，避免过度压缩或过度放大
        double smoothedMin = smoothed[0];
        double smoothedMax = smoothed[0];
        for (int i = 1; i < M; i++) {
            if (smoothed[i] < smoothedMin) smoothedMin = smoothed[i];
            if (smoothed[i] > smoothedMax) smoothedMax = smoothed[i];
        }
        
        // 计算原始数据的范围
        double rawMin = rawLogRho[0];
        double rawMax = rawLogRho[0];
        for (int i = 1; i < M; i++) {
            if (rawLogRho[i] < rawMin) rawMin = rawLogRho[i];
            if (rawLogRho[i] > rawMax) rawMax = rawLogRho[i];
        }
        
        // 如果滤波后的范围太小，适度扩大（但不完全恢复到原始范围）
        if (smoothedMax > smoothedMin + 1e-6 && rawMax > rawMin + 1e-6) {
            double smoothedRange = smoothedMax - smoothedMin;
            double rawRange = rawMax - rawMin;
            
            // 如果滤波后范围小于原始范围的20%，则适度扩大
            if (smoothedRange < rawRange * 0.2) {
                // 计算目标范围：原始范围的40-50%，保持平滑性，避免范围过大
                double targetRange = rawRange * 0.45;
                double smoothedCenter = (smoothedMin + smoothedMax) / 2.0;
                double rawCenter = (rawMin + rawMax) / 2.0;
                
                // 使用原始中心位置，但保持滤波后的相对分布
                double scale = targetRange / smoothedRange;
                for (int i = 0; i < M; i++) {
                    smoothed[i] = rawCenter + (smoothed[i] - smoothedCenter) * scale;
                    // 确保值在合理范围内
                    if (smoothed[i] < logMinRho) smoothed[i] = logMinRho;
                    if (smoothed[i] > logMaxRho) smoothed[i] = logMaxRho;
                }
            }
        }
        
        mLogRho = smoothed;
    } else {
        mLogRho = rawLogRho;
    }
}

void MTInversionCore::generateRandomModelAndForward(int M, int nFreq,
                                                    double minRho, double maxRho,
                                                    double firstThickness, double growthFactor,
                                                    double filterCutoff,
                                                    std::vector<double>& mLogRho,
                                                    std::vector<double>& layerThicknesses,
                                                    std::vector<double>& layerDepths,
                                                    std::vector<double>& periods,
                                                    std::vector<double>& omega,
                                                    std::vector<double>& dataOut) {
    // 计算层厚度
    computeLayerThicknesses(M, firstThickness, growthFactor, layerThicknesses, layerDepths);

    // 生成随机模型（带滤波）
    generateRandomModel(M, minRho, maxRho, filterCutoff, mLogRho);

    // 生成频率数组
    m_frequencyGenerator.generate(nFreq, periods, omega);

    // 正演计算
    m_forwardSolver.solve(mLogRho, omega, layerThicknesses, dataOut);
}

double MTInversionCore::gaussianRandom(double mean, double stddev) {
    static bool hasSpare = false;
    static double spare;

    if (hasSpare) {
        hasSpare = false;
        return mean + stddev * spare;
    }

    hasSpare = true;
    double u1 = static_cast<double>(rand()) / RAND_MAX;
    double u2 = static_cast<double>(rand()) / RAND_MAX;

    if (u1 < 1e-10) u1 = 1e-10;

    double mag = stddev * sqrt(-2.0 * log(u1));
    spare = mag * cos(2.0 * M_PI * u2);
    return mean + mag * sin(2.0 * M_PI * u2);
}
