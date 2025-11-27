cmake_minimum_required(VERSION 3.10)
project(Chapter07_Scientific_Computing)

# 设置UTF-8编码支持（处理中文路径）
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译器优化选项（根据构建类型）
# 注意：MSVC 的 /RTC1 与任何优化选项（/O1, /O2, /Ox）都不兼容
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang: 使用生成器表达式根据配置类型设置选项
    add_compile_options(
        "$<$<CONFIG:Debug>:-O0 -g>"
        "$<$<CONFIG:Release>:-O3>"
        "$<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${CMAKE_CROSSCOMPILING}>>>:-march=native>"
    )
    message(STATUS "Compiler optimization: Debug=-O0 -g, Release=-O3 -march=native (GCC/Clang)")
elseif(MSVC)
    # MSVC: 显式设置 Debug 和 Release 配置的编译标志
    # 这样可以覆盖 CMake 的默认设置，避免优化选项冲突
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /RTC1 /MDd /Zi" CACHE STRING "Debug compiler flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MD /DNDEBUG" CACHE STRING "Release compiler flags" FORCE)

    # AVX2 指令集优化（仅 Release 模式）
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX2" CACHE STRING "Release compiler flags" FORCE)
        message(STATUS "Compiler optimization: Debug=/Od /RTC1, Release=/O2 /arch:AVX2 (MSVC)")
    else()
        message(STATUS "Compiler optimization: Debug=/Od /RTC1, Release=/O2 (MSVC)")
    endif()
endif()

# 查找Eigen库（仅头文件）
find_path(EIGEN_INCLUDE_DIR Eigen/Dense
    PATHS ${CMAKE_SOURCE_DIR}/../../../third_party/eigen
          /usr/include/eigen3
          C:/Qt/eigen-3.4.0
          $ENV{EIGEN_ROOT}
)
if(EIGEN_INCLUDE_DIR)
    message(STATUS "Found Eigen: ${EIGEN_INCLUDE_DIR}")
    include_directories(${EIGEN_INCLUDE_DIR})
else()
    message(WARNING "Eigen not found. Please set EIGEN_ROOT or install Eigen.")
endif()

# 查找Boost库（可选）
find_package(Boost QUIET COMPONENTS math random)
if(Boost_FOUND)
    message(STATUS "Found Boost: ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(WARNING "Boost not found. Boost examples will be skipped.")
    set(Boost_FOUND FALSE)
endif()

# 查找Intel MKL
find_path(MKL_INCLUDE_DIR mkl.h
    PATHS $ENV{MKLROOT}/include
          $ENV{MKLROOT}/include/mkl
          /opt/intel/mkl/include
          /usr/include/mkl
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/include"
)
find_library(MKL_CORE mkl_core
    PATHS $ENV{MKLROOT}/lib $ENV{MKLROOT}/lib/intel64
          $ENV{MKLROOT}/lib/win-x86_64
          /opt/intel/mkl/lib/intel64
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64"
)
find_library(MKL_SEQUENTIAL mkl_sequential
    PATHS $ENV{MKLROOT}/lib $ENV{MKLROOT}/lib/intel64
          $ENV{MKLROOT}/lib/win-x86_64
          /opt/intel/mkl/lib/intel64
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64"
)
find_library(MKL_LP64 mkl_intel_lp64
    PATHS $ENV{MKLROOT}/lib $ENV{MKLROOT}/lib/intel64
          $ENV{MKLROOT}/lib/win-x86_64
          /opt/intel/mkl/lib/intel64
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64"
)

if(MKL_INCLUDE_DIR AND MKL_CORE AND MKL_SEQUENTIAL AND MKL_LP64)
    message(STATUS "Found Intel MKL: ${MKL_INCLUDE_DIR}")
    set(MKL_FOUND TRUE)
    include_directories(${MKL_INCLUDE_DIR})
else()
    message(WARNING "Intel MKL not found. Please set MKLROOT or install MKL.")
    set(MKL_FOUND FALSE)
endif()

# 设置输出目录，避免并行编译时的文件冲突
# MSVC 多配置生成器需要特殊处理
if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/RelWithDebInfo)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/MinSizeRel)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# 设置链接器选项，避免文件锁定问题
if(MSVC)
    # 使用 /INCREMENTAL:NO 禁用增量链接，避免文件锁定
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO")
endif()

# Eigen示例（只需要头文件）
if(EIGEN_INCLUDE_DIR)
    add_executable(eigen_basic eigen_basic.cpp)
    add_executable(eigen_data_types eigen_data_types.cpp)
    add_executable(eigen_matrix_creation eigen_matrix_creation.cpp)
    add_executable(eigen_matrix_arithmetic eigen_matrix_arithmetic.cpp)
    add_executable(eigen_element_access eigen_element_access.cpp)
    add_executable(eigen_block_operations eigen_block_operations.cpp)
    add_executable(eigen_transpose_conjugate eigen_transpose_conjugate.cpp)
    add_executable(eigen_norm_statistics eigen_norm_statistics.cpp)
    add_executable(eigen_matrix_ops eigen_matrix_ops.cpp)
    add_executable(eigen_solvers eigen_solvers.cpp)
    add_executable(eigen_eigenvalues eigen_eigenvalues.cpp)
    add_executable(eigen_performance_optimization eigen_performance_optimization.cpp)
    add_executable(eigen_numerical_stability eigen_numerical_stability.cpp)
    add_executable(eigen_debugging eigen_debugging.cpp)
    add_executable(eigen_comprehensive_usage eigen_comprehensive_usage.cpp)
    add_executable(eigen_array_operations eigen_array_operations.cpp)
    add_executable(eigen_sparse_matrix eigen_sparse_matrix.cpp)
    add_executable(eigen_geometry eigen_geometry.cpp)
    add_executable(eigen_decomposition_comparison eigen_decomposition_comparison.cpp)
    add_executable(eigen_advanced_operations eigen_advanced_operations.cpp)
    add_executable(eigen_best_practices eigen_best_practices.cpp)
    add_executable(eigen_expression_templates eigen_expression_templates.cpp)
    add_executable(eigen_map_stl eigen_map_stl.cpp)
    add_executable(eigen_applications eigen_applications.cpp)
    add_executable(eigen_linear_systems eigen_linear_systems.cpp)
    add_executable(eigen_simple_applications eigen_simple_applications.cpp)
endif()

# Boost示例
if(Boost_FOUND)
    add_executable(boost_math boost_math.cpp)
    target_link_libraries(boost_math Boost::math)

    add_executable(boost_math_special_functions boost_math_special_functions.cpp)
    target_link_libraries(boost_math_special_functions Boost::math)

    add_executable(boost_math_comprehensive boost_math_comprehensive.cpp)
    target_link_libraries(boost_math_comprehensive Boost::math)

    add_executable(boost_math_advanced boost_math_advanced.cpp)
    target_link_libraries(boost_math_advanced Boost::math)

    add_executable(boost_multiprecision boost_multiprecision.cpp)
    target_link_libraries(boost_multiprecision Boost::math)

    add_executable(boost_multiprecision_types boost_multiprecision_types.cpp)
    target_link_libraries(boost_multiprecision_types Boost::math)

    add_executable(boost_statistics boost_statistics.cpp)
    target_link_libraries(boost_statistics Boost::math)

    add_executable(boost_accumulators_examples boost_accumulators_examples.cpp)
    target_link_libraries(boost_accumulators_examples Boost::math)

    add_executable(boost_random boost_random.cpp)
    target_link_libraries(boost_random Boost::random)

    add_executable(boost_units boost_units.cpp)
    target_link_libraries(boost_units Boost::math)

    add_executable(boost_geometry boost_geometry.cpp)
    target_link_libraries(boost_geometry Boost::math)

    add_executable(boost_interval boost_interval.cpp)
    target_link_libraries(boost_interval Boost::math)
endif()

# Boost和Eigen集成示例
if(EIGEN_INCLUDE_DIR AND Boost_FOUND)
    add_executable(boost_eigen_integration boost_eigen_integration.cpp)
    target_link_libraries(boost_eigen_integration Boost::math Boost::random)
    target_include_directories(boost_eigen_integration PRIVATE ${EIGEN_INCLUDE_DIR})
endif()

# Intel MKL示例
if(MKL_FOUND)
    # Windows平台不需要pthread
    if(WIN32)
        set(MKL_LIBS ${MKL_LP64} ${MKL_CORE} ${MKL_SEQUENTIAL})
    else()
        set(MKL_LIBS ${MKL_LP64} ${MKL_CORE} ${MKL_SEQUENTIAL} pthread m dl)
    endif()

    add_executable(mkl_blas_level1 mkl_blas_level1.cpp)
    target_link_libraries(mkl_blas_level1 ${MKL_LIBS})

    add_executable(mkl_blas_level2 mkl_blas_level2.cpp)
    target_link_libraries(mkl_blas_level2 ${MKL_LIBS})

    add_executable(mkl_blas_level3 mkl_blas_level3.cpp)
    target_link_libraries(mkl_blas_level3 ${MKL_LIBS})

    add_executable(mkl_lapack mkl_lapack.cpp)
    target_link_libraries(mkl_lapack ${MKL_LIBS})

    add_executable(mkl_sparse mkl_sparse.cpp)
    target_link_libraries(mkl_sparse ${MKL_LIBS})

    add_executable(mkl_pardiso mkl_pardiso.cpp)
    # PARDISO需要额外的库链接
    if(WIN32)
        target_link_libraries(mkl_pardiso ${MKL_LIBS})
    else()
        target_link_libraries(mkl_pardiso ${MKL_LIBS})
    endif()

    add_executable(mkl_random mkl_random.cpp)
    target_link_libraries(mkl_random ${MKL_LIBS})

    add_executable(mkl_vml mkl_vml.cpp)
    target_link_libraries(mkl_vml ${MKL_LIBS})

    add_executable(mkl_dft mkl_dft.cpp)
    target_link_libraries(mkl_dft ${MKL_LIBS})

    add_executable(mkl_fftw3x mkl_fftw3x.cpp)
    target_link_libraries(mkl_fftw3x ${MKL_LIBS})

    add_executable(mkl_vsl_essl mkl_vsl_essl.cpp)
    target_link_libraries(mkl_vsl_essl ${MKL_LIBS})

    add_executable(mkl_sparse_blas mkl_sparse_blas.cpp)
    target_link_libraries(mkl_sparse_blas ${MKL_LIBS})

    add_executable(mkl_sparse_directsolvers mkl_sparse_directsolvers.cpp)
    target_link_libraries(mkl_sparse_directsolvers ${MKL_LIBS})

    add_executable(mkl_sparse_itersolvers mkl_sparse_itersolvers.cpp)
    target_link_libraries(mkl_sparse_itersolvers ${MKL_LIBS})

    add_executable(mkl_sparse_eigsolvers mkl_sparse_eigsolvers.cpp)
    target_link_libraries(mkl_sparse_eigsolvers ${MKL_LIBS})

    add_executable(mkl_statistics mkl_statistics.cpp)
    target_link_libraries(mkl_statistics ${MKL_LIBS})

    add_executable(mkl_data_fitting mkl_data_fitting.cpp)
    target_link_libraries(mkl_data_fitting ${MKL_LIBS})

    add_executable(mkl_nonlinear_solvers mkl_nonlinear_solvers.cpp)
    target_link_libraries(mkl_nonlinear_solvers ${MKL_LIBS})

    add_executable(mkl_service mkl_service.cpp)
    target_link_libraries(mkl_service ${MKL_LIBS})

    add_executable(mkl_trans mkl_trans.cpp)
    target_link_libraries(mkl_trans ${MKL_LIBS})

    add_executable(mkl_pdepoisson mkl_pdepoisson.cpp)
    target_link_libraries(mkl_pdepoisson ${MKL_LIBS})

    add_executable(mkl_pdett mkl_pdett.cpp)
    target_link_libraries(mkl_pdett ${MKL_LIBS})
endif()

# 库混合使用示例
if(EIGEN_INCLUDE_DIR AND MKL_FOUND)
    add_executable(eigen_mkl_integration eigen_mkl_integration.cpp)
    target_link_libraries(eigen_mkl_integration ${MKL_LIBS})
endif()


if(EIGEN_INCLUDE_DIR AND MKL_FOUND AND Boost_FOUND)
    add_executable(mixed_libraries_example mixed_libraries_example.cpp)
    if(WIN32)
        target_link_libraries(mixed_libraries_example ${MKL_LIBS} Boost::math Boost::random)
    else()
        target_link_libraries(mixed_libraries_example ${MKL_LIBS} Boost::math Boost::random)
    endif()
endif()

# 设置警告选项（优化选项已在上面设置）
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
elseif(MSVC)
    add_compile_options(/W4)  # UTF-8编码已在文件开头设置
endif()

# 启用OpenMP（如果可用）
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# 输出配置信息
message(STATUS "=== Chapter 7 Build Configuration ===")
message(STATUS "Eigen support: ${EIGEN_INCLUDE_DIR}")
message(STATUS "Boost support: ${Boost_FOUND}")
message(STATUS "MKL support: ${MKL_FOUND}")
message(STATUS "OpenMP support: ${OpenMP_CXX_FOUND}")
