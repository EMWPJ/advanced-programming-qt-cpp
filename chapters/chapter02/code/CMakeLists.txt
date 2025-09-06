cmake_minimum_required(VERSION 3.10)
project(Chapter02Examples)

# 设置编码选项
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

file(GLOB C_SOURCES "*.c")
file(GLOB CPP_SOURCES "*.cpp")

foreach(source ${C_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
endforeach()

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

foreach(source ${CPP_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    target_link_libraries(${name} Qt6::Core Qt6::Gui Qt6::Widgets)
endforeach()
