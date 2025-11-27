#include "mt_forward_solver.h"
#include <mkl.h>
#include <cmath>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MT {

ForwardSolver::ForwardSolver() {
}

ForwardSolver::~ForwardSolver() {
}

void ForwardSolver::solve(const std::vector<double>& mLogRho,
                          const std::vector<double>& omega,
                          const std::vector<double>& layerThicknesses,
                          std::vector<double>& dataOut) {
    int M = static_cast<int>(mLogRho.size());
    int nFreq = static_cast<int>(omega.size());
    int nData = nFreq * 2;

    dataOut.resize(nData);

    // 计算电导率
    std::vector<double> sigma(M);
    computeConductivity(mLogRho, sigma);

    // 使用提供的层厚度数组
    std::vector<double> dz = layerThicknesses;
    if (dz.empty() || dz.size() != static_cast<size_t>(M)) {
        // 如果未提供或大小不匹配，使用默认值
        dz.resize(M);
        double defaultDz = 100.0;
        for (int i = 0; i < M; i++) {
            dz[i] = defaultDz;
        }
    }

    // 对每个频率进行正演（使用向上递推阻抗的解析法）
    for (int ifreq = 0; ifreq < nFreq; ifreq++) {
        double w = omega[ifreq];
        
        // 使用向上递推阻抗法计算地表阻抗
        MKL_Complex16 Z_surface;
        computeRecursiveImpedance(M, w, sigma, dz, Z_surface);

        // 计算视电阻率：ρ_a = |Z|² / (ωμ₀)
        double Z_mag2 = Z_surface.real * Z_surface.real + Z_surface.imag * Z_surface.imag;
        double denom = w * MU0;
        double rho_a;  // 声明视电阻率变量
        // 检查除零和无效值
        if (denom <= 0.0 || !std::isfinite(denom) || !std::isfinite(Z_mag2)) {
            rho_a = 1e-10;  // 默认值
        } else {
            rho_a = Z_mag2 / denom;
        }
        
        // 检查NaN和Inf
        if (!std::isfinite(rho_a) || rho_a <= 0.0) {
            rho_a = 1e-10;
        }

        // 计算相位：φ = atan2(Im(Z), Re(Z))
        double phase_rad = atan2(Z_surface.imag, Z_surface.real);
        double phase = phase_rad * 180.0 / M_PI;

        // 输出：log10(视电阻率)和相位（度）
        int idx_rho = ifreq * 2;
        int idx_phase = ifreq * 2 + 1;
        dataOut[idx_rho] = log10(rho_a);
        dataOut[idx_phase] = phase;
    }
}

void ForwardSolver::solve(const ModelParams& model,
                         const std::vector<double>& omega,
                         std::vector<double>& dataOut) {
    solve(model.mLogRho, omega, model.layerThicknesses, dataOut);
}

void ForwardSolver::computeConductivity(const std::vector<double>& mLogRho,
                                         std::vector<double>& sigma) {
    int M = static_cast<int>(mLogRho.size());
    sigma.resize(M);

    // 使用VML计算10^mLogRho: 10^x = exp(x * ln(10))
    std::vector<double> rho(M);
    const double ln10 = log(10.0);  // ln(10)，使用标准库函数确保精度
    std::vector<double> base_array(M, ln10);  // 创建全为ln10的数组
    std::vector<double> mLogRho_times_ln10(M);
    // 使用VML的vdMul计算 mLogRho * ln(10)
    vdMul(M, mLogRho.data(), base_array.data(), mLogRho_times_ln10.data());
    // 然后使用VML的vdExp计算 exp(mLogRho * ln(10)) = 10^mLogRho
    vdExp(M, mLogRho_times_ln10.data(), rho.data());

    // 使用VML的vdInv进行sigma = 1/rho的向量化计算
    vdInv(M, rho.data(), sigma.data());
}

void ForwardSolver::computeRecursiveImpedance(int M, double w,
                                               const std::vector<double>& sigma,
                                               const std::vector<double>& dz,
                                               MKL_Complex16& Z_surface) {
    // 向上递推阻抗的解析法
    // 从最底层（半空间）开始，向上递推到地表
    
    // 最底层（第M-1层）的阻抗：Z_bottom = sqrt(iωμ₀/σ_bottom)
    // sqrt(i) = (1+i)/sqrt(2)，所以 sqrt(iωμ₀/σ) = (1+i) * sqrt(ωμ₀/(2σ))
    // 检查参数有效性
    if (M <= 0 || sigma.empty() || sigma[M - 1] <= 0.0 || w <= 0.0) {
        Z_surface.real = 1e-10;
        Z_surface.imag = 1e-10;
        return;
    }
    double denom = 2.0 * sigma[M - 1];
    if (denom <= 0.0 || !std::isfinite(denom)) {
        Z_surface.real = 1e-10;
        Z_surface.imag = 1e-10;
        return;
    }
    double sqrt_wmu0_2sigma = sqrt(w * MU0 / denom);
    if (!std::isfinite(sqrt_wmu0_2sigma)) {
        sqrt_wmu0_2sigma = 1e-10;
    }
    
    MKL_Complex16 Z_current;
    Z_current.real = sqrt_wmu0_2sigma;
    Z_current.imag = sqrt_wmu0_2sigma;
    
    // 从底层向上递推到地表
    for (int i = M - 2; i >= 0; i--) {
        // 检查数组边界和参数有效性
        if (i < 0 || i >= static_cast<int>(sigma.size()) || i >= static_cast<int>(dz.size()) || 
            sigma[i] <= 0.0 || dz[i] <= 0.0) {
            continue;  // 跳过无效层
        }
        
        // 第i层的特征阻抗：Z_0i = sqrt(iωμ₀/σ_i) = (1+i) * sqrt(ωμ₀/(2σ_i))
        double denom_i = 2.0 * sigma[i];
        if (denom_i <= 0.0 || !std::isfinite(denom_i)) {
            continue;  // 跳过无效层
        }
        double sqrt_wmu0_2sigma_i = sqrt(w * MU0 / denom_i);
        if (!std::isfinite(sqrt_wmu0_2sigma_i)) {
            sqrt_wmu0_2sigma_i = 1e-10;
        }
        MKL_Complex16 Z0_i;
        Z0_i.real = sqrt_wmu0_2sigma_i;
        Z0_i.imag = sqrt_wmu0_2sigma_i;
        
        // 第i层的波数：k_i = sqrt(iωμ₀σ_i) = (1+i) * sqrt(ωμ₀σ_i/2)
        double sqrt_wmu0_sigma_2 = sqrt(w * MU0 * sigma[i] / 2.0);
        if (!std::isfinite(sqrt_wmu0_sigma_2)) {
            sqrt_wmu0_sigma_2 = 1e-10;
        }
        MKL_Complex16 k_i;
        k_i.real = sqrt_wmu0_sigma_2;
        k_i.imag = sqrt_wmu0_sigma_2;
        
        // 计算 k_i * d_i
        double d_i = dz[i];
        if (!std::isfinite(d_i) || d_i <= 0.0) {
            continue;  // 跳过无效层
        }
        MKL_Complex16 kd;
        kd.real = k_i.real * d_i;
        kd.imag = k_i.imag * d_i;
        
        // 使用递推公式：Z_i = Z_0i * (Z_{i+1} + Z_0i * tanh(k_i * d_i)) / (Z_0i + Z_{i+1} * tanh(k_i * d_i))
        // 首先计算 tanh(k_i * d_i)
        double exp_real = exp(kd.real);
        double exp_minus_real = 1.0 / exp_real;
        double cos_imag = cos(kd.imag);
        double sin_imag = sin(kd.imag);
        
        MKL_Complex16 exp_kd, exp_minus_kd;
        exp_kd.real = exp_real * cos_imag;
        exp_kd.imag = exp_real * sin_imag;
        exp_minus_kd.real = exp_minus_real * cos_imag;
        exp_minus_kd.imag = -exp_minus_real * sin_imag;
        
        MKL_Complex16 sinh_kd, cosh_kd;
        sinh_kd.real = 0.5 * (exp_kd.real - exp_minus_kd.real);
        sinh_kd.imag = 0.5 * (exp_kd.imag - exp_minus_kd.imag);
        cosh_kd.real = 0.5 * (exp_kd.real + exp_minus_kd.real);
        cosh_kd.imag = 0.5 * (exp_kd.imag + exp_minus_kd.imag);
        
        double cosh_mag2 = cosh_kd.real * cosh_kd.real + cosh_kd.imag * cosh_kd.imag;
        MKL_Complex16 tanh_kd;
        if (cosh_mag2 > 1e-20) {
            tanh_kd.real = (sinh_kd.real * cosh_kd.real + sinh_kd.imag * cosh_kd.imag) / cosh_mag2;
            tanh_kd.imag = (sinh_kd.imag * cosh_kd.real - sinh_kd.real * cosh_kd.imag) / cosh_mag2;
        } else {
            tanh_kd.real = 1.0;
            tanh_kd.imag = 0.0;
        }
        
        // 计算 Z_0i * tanh(k_i * d_i)
        MKL_Complex16 Z0_tanh;
        Z0_tanh.real = Z0_i.real * tanh_kd.real - Z0_i.imag * tanh_kd.imag;
        Z0_tanh.imag = Z0_i.real * tanh_kd.imag + Z0_i.imag * tanh_kd.real;
        
        // 计算 Z_{i+1} + Z_0i * tanh(k_i * d_i)
        MKL_Complex16 numerator;
        numerator.real = Z_current.real + Z0_tanh.real;
        numerator.imag = Z_current.imag + Z0_tanh.imag;
        
        // 计算 Z_0i + Z_{i+1} * tanh(k_i * d_i)
        MKL_Complex16 Z_current_tanh;
        Z_current_tanh.real = Z_current.real * tanh_kd.real - Z_current.imag * tanh_kd.imag;
        Z_current_tanh.imag = Z_current.real * tanh_kd.imag + Z_current.imag * tanh_kd.real;
        
        MKL_Complex16 denominator;
        denominator.real = Z0_i.real + Z_current_tanh.real;
        denominator.imag = Z0_i.imag + Z_current_tanh.imag;
        
        // 计算 Z_i = Z_0i * numerator / denominator
        double denom_mag2 = denominator.real * denominator.real + denominator.imag * denominator.imag;
        MKL_Complex16 ratio;
        if (denom_mag2 > 1e-20) {
            ratio.real = (numerator.real * denominator.real + numerator.imag * denominator.imag) / denom_mag2;
            ratio.imag = (numerator.imag * denominator.real - numerator.real * denominator.imag) / denom_mag2;
        } else {
            ratio.real = 1.0;
            ratio.imag = 0.0;
        }
        
        Z_current.real = Z0_i.real * ratio.real - Z0_i.imag * ratio.imag;
        Z_current.imag = Z0_i.real * ratio.imag + Z0_i.imag * ratio.real;
    }
    
    // 返回地表阻抗
    Z_surface = Z_current;
}

} // namespace MT

