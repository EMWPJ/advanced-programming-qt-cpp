#define _USE_MATH_DEFINES  // Windows上需要这个来使用M_PI
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cmath>
#define NOMINMAX
#include <windows.h>

using namespace std;
using namespace Eigen;

// Eigen几何变换完整示例

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "=== Eigen几何变换完整示例 ===" << endl;

    // ========== 1. 旋转：轴角表示 ==========
    cout << "\n--- 1. 旋转：轴角表示 ---" << endl;

    // 绕Z轴旋转45度
    AngleAxisd rotation_z(M_PI / 4, Vector3d::UnitZ());
    Matrix3d R_z = rotation_z.toRotationMatrix();
    cout << "绕Z轴旋转45度的旋转矩阵:\n" << R_z << endl;

    // 旋转一个点
    Vector3d point(1, 0, 0);
    Vector3d rotated_point = rotation_z * point;
    cout << "点(1,0,0)旋转后:\n" << rotated_point << endl;

    // 绕任意轴旋转
    Vector3d axis(1, 1, 1);
    axis.normalize();
    AngleAxisd rotation_axis(M_PI / 3, axis);
    cout << "绕轴(1,1,1)旋转60度" << endl;

    // ========== 2. 旋转：四元数表示 ==========
    cout << "\n--- 2. 旋转：四元数表示 ---" << endl;

    // 从轴角创建四元数
    Quaterniond q1(rotation_z);
    cout << "四元数 (w,x,y,z): (" << q1.w() << ", " << q1.x()
         << ", " << q1.y() << ", " << q1.z() << ")" << endl;

    // 四元数旋转
    Vector3d point2(1, 0, 0);
    Vector3d rotated2 = q1 * point2;
    cout << "用四元数旋转点(1,0,0):\n" << rotated2 << endl;

    // 四元数乘法（组合旋转）
    Quaterniond q2 = Quaterniond(AngleAxisd(M_PI / 6, Vector3d::UnitY()));
    Quaterniond q_combined = q2 * q1;
    cout << "组合旋转（先绕Z轴45度，再绕Y轴30度）" << endl;

    // ========== 3. 旋转：欧拉角 ==========
    cout << "\n--- 3. 旋转：欧拉角 ---" << endl;

    // ZYX欧拉角（Roll-Pitch-Yaw）
    double roll = M_PI / 6;   // 30度
    double pitch = M_PI / 4;  // 45度
    double yaw = M_PI / 3;    // 60度

    Matrix3d R_euler;
    R_euler = AngleAxisd(yaw, Vector3d::UnitZ()) *
              AngleAxisd(pitch, Vector3d::UnitY()) *
              AngleAxisd(roll, Vector3d::UnitX());

    cout << "欧拉角旋转矩阵 (Roll=" << roll << ", Pitch="
         << pitch << ", Yaw=" << yaw << "):\n" << R_euler << endl;

    // 从旋转矩阵提取欧拉角
    Vector3d euler_angles = R_euler.eulerAngles(2, 1, 0);  // ZYX顺序
    cout << "提取的欧拉角 (ZYX):\n" << euler_angles << endl;

    // ========== 4. 平移 ==========
    cout << "\n--- 4. 平移 ---" << endl;

    Translation3d translation(1.0, 2.0, 3.0);
    Vector3d point3(0, 0, 0);
    Vector3d translated = translation * point3;
    cout << "平移向量: (1, 2, 3)" << endl;
    cout << "点(0,0,0)平移后:\n" << translated << endl;

    // ========== 5. 缩放 ==========
    cout << "\n--- 5. 缩放 ---" << endl;

    DiagonalMatrix<double, 3> scaling(Vector3d(2.0, 3.0, 1.0));
    Vector3d point4(1, 1, 1);
    Vector3d scaled = scaling * point4;
    cout << "缩放因子: (2, 3, 1)" << endl;
    cout << "点(1,1,1)缩放后:\n" << scaled << endl;

    // 均匀缩放
    UniformScaling<double> uniform_scaling(2.0);
    Vector3d uniformly_scaled = uniform_scaling * point4;
    cout << "均匀缩放2倍后:\n" << uniformly_scaled << endl;

    // ========== 6. 仿射变换 ==========
    cout << "\n--- 6. 仿射变换 ---" << endl;

    // 组合旋转和平移
    Affine3d transform = Translation3d(1, 2, 3) * rotation_z;
    cout << "仿射变换矩阵（先旋转后平移）:\n" << transform.matrix() << endl;

    Vector3d point5(1, 0, 0);
    Vector3d transformed = transform * point5;
    cout << "点(1,0,0)变换后:\n" << transformed << endl;

    // 另一种方式：先旋转后平移
    Affine3d transform2 = Affine3d::Identity();
    transform2.translate(Vector3d(1, 2, 3));
    transform2.rotate(rotation_z);
    cout << "另一种组合方式（先平移后旋转）:\n" << transform2.matrix() << endl;

    // ========== 7. 投影变换 ==========
    cout << "\n--- 7. 投影变换 ---" << endl;

    // 透视投影矩阵（简化示例）
    Projective3d proj = Projective3d::Identity();
    // 这里只是示例，实际透视投影需要更复杂的矩阵
    cout << "投影变换矩阵:\n" << proj.matrix() << endl;

    // ========== 8. 变换组合 ==========
    cout << "\n--- 8. 变换组合 ---" << endl;

    // 创建一系列变换
    AngleAxisd rot1(M_PI / 4, Vector3d::UnitZ());
    Translation3d trans1(1, 0, 0);
    DiagonalMatrix<double, 3> scale1(Vector3d(2, 2, 1));

    // 组合变换：先缩放，再旋转，最后平移
    Affine3d combined = trans1 * rot1 * scale1;
    cout << "组合变换矩阵（缩放->旋转->平移）:\n" << combined.matrix() << endl;

    Vector3d test_point(1, 1, 0);
    Vector3d result_point = combined * test_point;
    cout << "点(1,1,0)变换后:\n" << result_point << endl;

    // ========== 9. 逆变换 ==========
    cout << "\n--- 9. 逆变换 ---" << endl;

    Affine3d T = trans1 * rot1;
    Affine3d T_inv = T.inverse();

    Vector3d original(1, 0, 0);
    Vector3d transformed_point = T * original;
    Vector3d restored = T_inv * transformed_point;

    cout << "原始点:\n" << original << endl;
    cout << "变换后:\n" << transformed_point << endl;
    cout << "逆变换恢复:\n" << restored << endl;

    // ========== 10. 坐标系变换 ==========
    cout << "\n--- 10. 坐标系变换 ---" << endl;

    // 定义两个坐标系
    // 坐标系1：原点在(0,0,0)，标准方向
    // 坐标系2：原点在(1,1,1)，绕Z轴旋转45度

    Affine3d T_1_to_2 = Translation3d(1, 1, 1) *
                        AngleAxisd(M_PI / 4, Vector3d::UnitZ());

    Vector3d point_in_frame1(1, 0, 0);
    Vector3d point_in_frame2 = T_1_to_2 * point_in_frame1;

    cout << "坐标系1中的点:\n" << point_in_frame1 << endl;
    cout << "在坐标系2中的坐标:\n" << point_in_frame2 << endl;

    // 转换回坐标系1
    Affine3d T_2_to_1 = T_1_to_2.inverse();
    Vector3d back_to_frame1 = T_2_to_1 * point_in_frame2;
    cout << "转换回坐标系1:\n" << back_to_frame1 << endl;

    // ========== 11. 2D变换 ==========
    cout << "\n--- 11. 2D变换 ---" << endl;

    // 2D旋转
    Rotation2D<double> rot_2d(M_PI / 4);
    Vector2d point_2d(1, 0);
    Vector2d rotated_2d = rot_2d * point_2d;
    cout << "2D点(1,0)旋转45度后:\n" << rotated_2d << endl;

    // 2D仿射变换
    Affine2d transform_2d = Translation2d(1, 1) * rot_2d;
    Vector2d transformed_2d = transform_2d * point_2d;
    cout << "2D点(1,0)变换后:\n" << transformed_2d << endl;

    // ========== 12. 实际应用：机器人运动学 ==========
    cout << "\n--- 12. 实际应用：机器人运动学 ---" << endl;

    // 简单的2D机器人（两个关节）
    // 第一个关节：绕原点旋转
    double theta1 = M_PI / 4;
    Affine2d joint1 = Affine2d::Identity();
    joint1.rotate(Rotation2D<double>(theta1));

    // 第二个关节：在第一个关节末端，再旋转
    double theta2 = M_PI / 6;
    Affine2d joint2 = Affine2d::Identity();
    joint2.translate(Vector2d(1, 0));
    joint2.rotate(Rotation2D<double>(theta2));

    // 末端执行器位置
    Affine2d end_effector = joint1 * joint2;
    end_effector.translate(Vector2d(0.5, 0));
    Vector2d end_pos = end_effector * Vector2d(0, 0);

    cout << "机器人末端执行器位置:\n" << end_pos << endl;

    cout << "\n=== 几何变换示例完成 ===" << endl;
    cin.get();
    return 0;
}
