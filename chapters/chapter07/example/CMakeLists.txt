cmake_minimum_required(VERSION 3.16)
project(MT1D_Inversion_Example)

# 设置UTF-8编码支持（处理中文路径）
if(MSVC)
    add_compile_options(/utf-8)
    # 启用多处理器编译
    add_compile_options(/MP)
    # 禁用一些有问题的警告
    add_compile_options(/wd4996)  # 禁用已弃用函数警告
    add_compile_options(/wd4819)  # 禁用代码页警告（已通过/utf-8处理）
    # 注意：/EHsc（C++异常处理）将仅应用于C++源文件，不在此处全局设置
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()


# 设置C++标准（默认使用C++17，与Qt兼容性更好）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译器优化选项（根据构建类型）
if(MSVC)
    set(CMAKE_C_FLAGS_DEBUG "/Od /RTC1 /MDd /Zi" CACHE STRING "Debug compiler flags" FORCE)
    set(CMAKE_C_FLAGS_RELEASE "/O2 /MD /DNDEBUG" CACHE STRING "Release compiler flags" FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /RTC1 /MDd /Zi" CACHE STRING "Debug C++ compiler flags" FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MD /DNDEBUG" CACHE STRING "Release C++ compiler flags" FORCE)

    # AVX2 指令集优化（仅 Release 模式）
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /arch:AVX2" CACHE STRING "Release compiler flags" FORCE)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX2" CACHE STRING "Release C++ compiler flags" FORCE)
        message(STATUS "Compiler optimization: Debug=/Od /RTC1, Release=/O2 /arch:AVX2 (MSVC)")
    else()
        message(STATUS "Compiler optimization: Debug=/Od /RTC1, Release=/O2 (MSVC)")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        "$<$<CONFIG:Debug>:-O0 -g>"
        "$<$<CONFIG:Release>:-O3>"
        "$<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${CMAKE_CROSSCOMPILING}>>>:-march=native>"
    )
    message(STATUS "Compiler optimization: Debug=-O0 -g, Release=-O3 -march=native (GCC/Clang)")
endif()

# 查找Intel MKL
find_path(MKL_INCLUDE_DIR mkl.h
    PATHS $ENV{MKLROOT}/include
          $ENV{MKLROOT}/include/mkl
          /opt/intel/mkl/include
          /usr/include/mkl
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/include"
          "C:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/include"
          "C:/Program Files/Intel/oneAPI/mkl/latest/include"
)

find_library(MKL_CORE mkl_core
    PATHS $ENV{MKLROOT}/lib
          $ENV{MKLROOT}/lib/intel64
          $ENV{MKLROOT}/lib/win-x86_64
          /opt/intel/mkl/lib/intel64
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64"
          "C:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/lib/intel64"
          "C:/Program Files/Intel/oneAPI/mkl/latest/lib/intel64"
)

find_library(MKL_SEQUENTIAL mkl_sequential
    PATHS $ENV{MKLROOT}/lib
          $ENV{MKLROOT}/lib/intel64
          $ENV{MKLROOT}/lib/win-x86_64
          /opt/intel/mkl/lib/intel64
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64"
          "C:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/lib/intel64"
          "C:/Program Files/Intel/oneAPI/mkl/latest/lib/intel64"
)

find_library(MKL_LP64 mkl_intel_lp64
    PATHS $ENV{MKLROOT}/lib
          $ENV{MKLROOT}/lib/intel64
          $ENV{MKLROOT}/lib/win-x86_64
          /opt/intel/mkl/lib/intel64
          /usr/lib/x86_64-linux-gnu
          "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries/windows/mkl/lib/intel64"
          "C:/Program Files (x86)/Intel/oneAPI/mkl/2025.0/lib/intel64"
          "C:/Program Files/Intel/oneAPI/mkl/latest/lib/intel64"
)

if(MKL_INCLUDE_DIR AND MKL_CORE AND MKL_SEQUENTIAL AND MKL_LP64)
    message(STATUS "Found Intel MKL: ${MKL_INCLUDE_DIR}")
    set(MKL_FOUND TRUE)
    include_directories(${MKL_INCLUDE_DIR})
    # Compose MKL link libraries so we can refer to a single variable later
    set(MKL_LIBS ${MKL_LP64} ${MKL_SEQUENTIAL} ${MKL_CORE})
    message(STATUS "Configured MKL libraries: ${MKL_LIBS}")
else()
    message(WARNING "Intel MKL not found. Please set MKLROOT or install MKL.")
    message(WARNING "You can set MKLROOT environment variable to point to your MKL installation.")
    set(MKL_FOUND FALSE)
endif()

# 设置输出目录
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
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO")
endif()

# 查找Qt库 - 使用更稳健的方法避免中文路径问题
message(STATUS "Searching for Qt...")
set(QT_FOUND FALSE)
set(QT_CHARTS_FOUND FALSE)

# 设置常见的Qt安装路径，避免在中文路径下搜索导致崩溃
set(QT_PATHS
    "C:/Qt"
    "C:/Qt/6.9.1/msvc2022_64"
    "C:/Qt/6.9.0/msvc2022_64"
    "C:/Qt/6.8.0/msvc2022_64"
    "$ENV{QT_DIR}"
    "$ENV{QTDIR}"
    "$ENV{CMAKE_PREFIX_PATH}"
)

# 尝试设置Qt6_DIR以避免在中文路径下搜索
foreach(QT_PATH ${QT_PATHS})
    if(EXISTS "${QT_PATH}/lib/cmake/Qt6")
        set(Qt6_DIR "${QT_PATH}/lib/cmake/Qt6" CACHE PATH "Qt6 cmake directory")
        message(STATUS "Found Qt6 installation at: ${QT_PATH}")
        break()
    endif()
endforeach()

# 如果未找到Qt6，尝试Qt5
if(NOT Qt6_DIR)
    foreach(QT_PATH ${QT_PATHS})
        if(EXISTS "${QT_PATH}/lib/cmake/Qt5")
            set(Qt5_DIR "${QT_PATH}/lib/cmake/Qt5" CACHE PATH "Qt5 cmake directory")
            message(STATUS "Found Qt5 installation at: ${QT_PATH}")
            break()
        endif()
    endforeach()
endif()

# 使用最小化的find_package调用，只查找Core组件
if(Qt6_DIR)
    message(STATUS "Attempting to find Qt6 using Qt6_DIR: ${Qt6_DIR}")
    find_package(Qt6 QUIET COMPONENTS Core)
    if(Qt6_FOUND)
        message(STATUS "Found Qt6: ${Qt6_VERSION}")
        # 现在查找Widgets组件
        find_package(Qt6 QUIET COMPONENTS Widgets)
        if(Qt6Widgets_FOUND)
            set(QT_FOUND TRUE)
            set(QT_VERSION_MAJOR 6)
            # 尝试查找Charts组件（可选）
            find_package(Qt6 QUIET COMPONENTS Charts)
            if(Qt6Charts_FOUND)
                message(STATUS "Found Qt6 Charts component")
                set(QT_CHARTS_FOUND TRUE)
                set(QT_LIBRARIES Qt6::Core Qt6::Widgets Qt6::Charts)
            else()
                message(STATUS "Qt6 Charts component not found, building without Charts")
                set(QT_LIBRARIES Qt6::Core Qt6::Widgets)
            endif()
        else()
            message(WARNING "Qt6 Core found but Widgets not found")
        endif()
    endif()
endif()

# 如果Qt6未找到，尝试Qt5
if(NOT QT_FOUND AND Qt5_DIR)
    message(STATUS "Attempting to find Qt5 using Qt5_DIR: ${Qt5_DIR}")
    find_package(Qt5 QUIET COMPONENTS Core)
    if(Qt5_FOUND)
        message(STATUS "Found Qt5: ${Qt5_VERSION}")
        find_package(Qt5 QUIET COMPONENTS Widgets)
        if(Qt5Widgets_FOUND)
            set(QT_FOUND TRUE)
            set(QT_VERSION_MAJOR 5)
            # 尝试查找Charts组件（可选）
            find_package(Qt5 QUIET COMPONENTS Charts)
            if(Qt5Charts_FOUND)
                message(STATUS "Found Qt5 Charts component")
                set(QT_CHARTS_FOUND TRUE)
                set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Charts)
            else()
                message(STATUS "Qt5 Charts component not found, building without Charts")
                set(QT_LIBRARIES Qt5::Core Qt5::Widgets)
            endif()
        else()
            message(WARNING "Qt5 Core found but Widgets not found")
        endif()
    endif()
endif()

# 如果仍未找到，尝试默认搜索（可能会慢或有风险）
if(NOT QT_FOUND)
    message(STATUS "Qt not found in common paths, trying default search...")
    message(WARNING "This may take a while and could fail with Chinese paths")
    find_package(Qt6 QUIET COMPONENTS Core Widgets)
    if(Qt6_FOUND)
        set(QT_FOUND TRUE)
        set(QT_VERSION_MAJOR 6)
        find_package(Qt6 QUIET COMPONENTS Charts)
        if(Qt6Charts_FOUND)
            set(QT_CHARTS_FOUND TRUE)
            set(QT_LIBRARIES Qt6::Core Qt6::Widgets Qt6::Charts)
        else()
            set(QT_LIBRARIES Qt6::Core Qt6::Widgets)
        endif()
    else()
        find_package(Qt5 QUIET COMPONENTS Core Widgets)
        if(Qt5_FOUND)
            set(QT_FOUND TRUE)
            set(QT_VERSION_MAJOR 5)
            find_package(Qt5 QUIET COMPONENTS Charts)
            if(Qt5Charts_FOUND)
                set(QT_CHARTS_FOUND TRUE)
                set(QT_LIBRARIES Qt5::Core Qt5::Widgets Qt5::Charts)
            else()
                set(QT_LIBRARIES Qt5::Core Qt5::Widgets)
            endif()
        endif()
    endif()
endif()

if(QT_FOUND)
    message(STATUS "Qt libraries to link: ${QT_LIBRARIES}")
else()
    message(WARNING "Qt not found. GUI version will not be built.")
    message(WARNING "Please install Qt6 or Qt5 and set Qt6_DIR or Qt5_DIR CMake variable.")
endif()

# MT一维反演GUI版本（C++/Qt）
if(MKL_FOUND AND QT_FOUND)
    # 添加GUI可执行文件（包含所有模块化组件）
    add_executable(mt1d_inversion_gui
        # 核心模块
        mt_inversion_core.cpp
        mt_inversion_core.h
        # 数据模型模块
        mt_model.h
        # 频率生成器模块
        mt_frequency_generator.cpp
        mt_frequency_generator.h
        # 正演求解器模块
        mt_forward_solver.cpp
        mt_forward_solver.h
        # Jacobian计算器模块
        mt_jacobian_calculator.cpp
        mt_jacobian_calculator.h
        # 正则化模块
        mt_regularization.cpp
        mt_regularization.h
        # 优化求解器模块
        mt_optimizer.cpp
        mt_optimizer.h
        # GUI模块
        mt_inversion_gui.cpp
        mt_inversion_gui.h
    )

    # 设置target级别的C++标准（确保使用C++17）
    set_target_properties(mt1d_inversion_gui PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    # 设置target级别的C++编译选项
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(mt1d_inversion_gui PRIVATE -Wall -Wextra)
    elseif(MSVC)
        # /EHsc: 启用C++异常处理（同步异常模型）
        target_compile_options(mt1d_inversion_gui PRIVATE /EHsc /W4)
    endif()

    # 链接MKL库
    target_link_libraries(mt1d_inversion_gui ${MKL_LIBS})

    # 链接Qt库
    target_link_libraries(mt1d_inversion_gui ${QT_LIBRARIES})
    set_target_properties(mt1d_inversion_gui PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
    )

    # 设置包含目录
    target_include_directories(mt1d_inversion_gui PRIVATE ${MKL_INCLUDE_DIR})

    message(STATUS "MT1D Inversion GUI example will be built")
else()
    if(NOT MKL_FOUND)
        message(WARNING "MT1D Inversion GUI will NOT be built (MKL not found)")
    endif()
    if(NOT QT_FOUND)
        message(WARNING "MT1D Inversion GUI will NOT be built (Qt not found)")
    endif()
endif()

# 输出配置信息
message(STATUS "=== MT1D Inversion Build Configuration ===")
message(STATUS "MKL support: ${MKL_FOUND}")
if(MKL_FOUND)
    message(STATUS "MKL include: ${MKL_INCLUDE_DIR}")
    message(STATUS "MKL libraries: ${MKL_LIBS}")
endif()
message(STATUS "Qt support: ${QT_FOUND}")
if(QT_FOUND)
    if(QT_VERSION_MAJOR EQUAL 6)
        message(STATUS "Qt version: ${Qt6_VERSION}")
    elseif(QT_VERSION_MAJOR EQUAL 5)
        message(STATUS "Qt version: ${Qt5_VERSION}")
    endif()
    message(STATUS "Qt Charts support: ${QT_CHARTS_FOUND}")
endif()
