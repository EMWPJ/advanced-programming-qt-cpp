#ifndef MKL_BLAS_COMMON_H
#define MKL_BLAS_COMMON_H

#include <iostream>
#include <vector>
#include <chrono>
#include <complex>
#include <iomanip>
#include <string>
#include <cmath>
#include <mkl.h>
#include <windows.h>

using namespace std;
using namespace chrono;

// 打印矩阵（双精度）
inline void print_matrix(const char* name, const double* matrix, int rows, int cols) {
    cout << name << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i * cols + j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

// 打印矩阵（单精度）
inline void print_matrix(const char* name, const float* matrix, int rows, int cols) {
    cout << name << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i * cols + j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

// 打印向量（双精度）
inline void print_vector(const char* name, const double* vector, int size) {
    cout << name << ":" << endl;
    for (int i = 0; i < size; ++i) {
        cout << vector[i] << " ";
    }
    cout << endl << endl;
}

// 打印向量（单精度）
inline void print_vector(const char* name, const float* vector, int size) {
    cout << name << ":" << endl;
    for (int i = 0; i < size; ++i) {
        cout << vector[i] << " ";
    }
    cout << endl << endl;
}

// 打印复数矩阵（双精度）
inline void print_complex_matrix(const char* name, const MKL_Complex16* matrix, int rows, int cols) {
    cout << name << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx = i * cols + j;
            cout << "(" << matrix[idx].real << ", " << matrix[idx].imag << ")\t";
        }
        cout << endl;
    }
    cout << endl;
}

// 打印复数矩阵（单精度）
inline void print_complex_matrix(const char* name, const MKL_Complex8* matrix, int rows, int cols) {
    cout << name << ":" << endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int idx = i * cols + j;
            cout << "(" << matrix[idx].real << ", " << matrix[idx].imag << ")\t";
        }
        cout << endl;
    }
    cout << endl;
}

// 初始化控制台输出编码
inline void init_console() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

#endif // MKL_BLAS_COMMON_H
