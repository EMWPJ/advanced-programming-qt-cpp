cmake_minimum_required(VERSION 3.5)

project(Chapter03QtExamples)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
  add_compile_options(/utf-8)
endif()

file(GLOB CPP_SOURCES "*.cpp")

find_package(Qt6 COMPONENTS Widgets Core Gui Concurrent REQUIRED)
foreach(source ${CPP_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    target_link_libraries(${name} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)
endforeach()