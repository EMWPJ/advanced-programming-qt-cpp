#ifndef MT_REGULARIZATION_H
#define MT_REGULARIZATION_H

#include "mt_model.h"
#include <vector>
#include <string>

/**
 * MT正则化模块
 * 负责构建正则化矩阵，用于反演中的模型平滑约束
 */
namespace MT {

class Regularization {
public:
    /**
     * 正则化类型枚举
     */
    enum class Type {
        SMOOTHNESS,      // 平滑度约束（二阶差分）
        FLATNESS,        // 平坦度约束（一阶差分）
        MINIMUM_NORM     // 最小范数约束（单位矩阵）
    };

    Regularization();
    ~Regularization();

    /**
     * 构建正则化矩阵L
     * @param M 模型参数个数
     * @param type 正则化类型
     * @param L 输出的正则化矩阵（L_rows行×M列）
     */
    void buildLMatrix(int M, Type type, std::vector<std::vector<double>>& L);

    /**
     * 构建正则化矩阵L（使用默认类型：平滑度约束）
     * @param M 模型参数个数
     * @param L 输出的正则化矩阵
     */
    void buildLMatrix(int M, std::vector<std::vector<double>>& L);

    /**
     * 计算L^T * L
     * @param L 正则化矩阵L
     * @param LTL 输出的L^T*L矩阵（M×M对称矩阵）
     */
    void computeLTL(const std::vector<std::vector<double>>& L,
                    std::vector<std::vector<double>>& LTL);

    /**
     * 设置正则化类型
     * @param type 正则化类型
     */
    void setType(Type type);

    /**
     * 获取正则化类型
     * @return 正则化类型
     */
    Type getType() const { return m_type; }

private:
    /**
     * 构建平滑度约束矩阵（二阶差分）
     * @param M 模型参数个数
     * @param L 输出的正则化矩阵
     */
    void buildSmoothnessMatrix(int M, std::vector<std::vector<double>>& L);

    /**
     * 构建平坦度约束矩阵（一阶差分）
     * @param M 模型参数个数
     * @param L 输出的正则化矩阵
     */
    void buildFlatnessMatrix(int M, std::vector<std::vector<double>>& L);

    /**
     * 构建最小范数约束矩阵（单位矩阵）
     * @param M 模型参数个数
     * @param L 输出的正则化矩阵
     */
    void buildMinimumNormMatrix(int M, std::vector<std::vector<double>>& L);

    Type m_type;  // 正则化类型
};

} // namespace MT

#endif // MT_REGULARIZATION_H

