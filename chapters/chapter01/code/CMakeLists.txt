# CMakeLists.txt 配置示例
cmake_minimum_required(VERSION 3.16)  # 指定所需的最低CMake版本
project(FirstApp)  # 定义项目名称
# 设置C++标准为17，并强制要求
set(CMAKE_CXX_STANDARD 17)                # 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)       # 强制使用指定的C++标准
# 根据不同编译器设置编译选项
if(MSVC)
    # MSVC（微软编译器）下设置警告级别为4
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    # GCC或Clang下开启所有警告和额外警告
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
# 查找并链接Qt6的Core和Widgets模块
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)  # 查找Qt6核心和窗口部件模块
# 添加可执行文件，指定源文件
add_executable(FirstApp main.cpp)  # 生成名为FirstApp的可执行文件，源文件为main.cpp
# 链接Qt6库到目标程序
target_link_libraries(FirstApp Qt6::Core Qt6::Widgets)  # 链接Qt6的Core和Widgets库
