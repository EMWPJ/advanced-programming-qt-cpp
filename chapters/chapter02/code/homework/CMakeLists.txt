cmake_minimum_required(VERSION 3.16)
project(LinearSystemSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置UTF-8编码支持
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/D_UNICODE>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/DUNICODE>")

# 添加可执行文件
add_executable(linear_system_solver
    main.cpp
    matrix.cpp
    vector.cpp
    gaussian_solver.cpp
    lu_solver.cpp
    jacobi_solver.cpp
    sor_solver.cpp
    linear_system.cpp
)

# 包含头文件目录
target_include_directories(linear_system_solver PRIVATE .)