#include "mt_frequency_generator.h"
#include <mkl_vml.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace MT {

FrequencyGenerator::FrequencyGenerator() {
}

FrequencyGenerator::~FrequencyGenerator() {
}

void FrequencyGenerator::generate(int nFreq, double T_min, double T_max,
                                   std::vector<double>& periods, std::vector<double>& omega) {
    periods.resize(nFreq);
    omega.resize(nFreq);

    // 计算 log10(T_min) 和 log10(T_max) 的等间隔向量
    std::vector<double> log_T_vec(nFreq);
    double log_T_min, log_T_max, dlog_T;
    int n_one = 1;
    vdLog10(n_one, &T_min, &log_T_min);  // log_T_min = log10(T_min)
    vdLog10(n_one, &T_max, &log_T_max);  // log_T_max = log10(T_max)
    dlog_T = (log_T_max - log_T_min) / (nFreq - 1);

    for (int i = 0; i < nFreq; i++) {
        log_T_vec[i] = log_T_min + i * dlog_T;
    }
    // 向量化计算 periods: periods = 10 ^ log_T_vec
    // 使用 vdExp: 10^x = exp(x * ln(10))
    const double ln10 = log(10.0);  // ln(10)，使用标准库函数确保精度
    std::vector<double> base_array(nFreq, ln10);  // 创建全为ln10的数组
    std::vector<double> log_T_vec_times_ln10(nFreq);
    // 使用VML的vdMul计算 log_T_vec * ln(10)
    vdMul(nFreq, log_T_vec.data(), base_array.data(), log_T_vec_times_ln10.data());
    // 然后使用VML的vdExp计算 exp(log_T_vec * ln(10)) = 10^log_T_vec
    vdExp(nFreq, log_T_vec_times_ln10.data(), periods.data());

    // omega = 2*pi / period 向量化计算
    std::vector<double> periodInv(nFreq);
    vdInv(nFreq, periods.data(), periodInv.data());
    double factor = 2.0 * M_PI;
    for (int i = 0; i < nFreq; i++) {
        omega[i] = factor * periodInv[i];
    }
}

void FrequencyGenerator::generate(int nFreq, std::vector<double>& periods, std::vector<double>& omega) {
    generate(nFreq, DEFAULT_T_MIN, DEFAULT_T_MAX, periods, omega);
}

FrequencyParams FrequencyGenerator::generateParams(int nFreq, double T_min, double T_max) {
    FrequencyParams params;
    params.nFreq = nFreq;
    params.T_min = T_min;
    params.T_max = T_max;
    generate(nFreq, T_min, T_max, params.periods, params.omega);
    return params;
}

} // namespace MT

