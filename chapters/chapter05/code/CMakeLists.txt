cmake_minimum_required(VERSION 3.5)

project(Chapter05QtExamples)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
  add_compile_options(/utf-8 /std:c++20)
else()
  add_compile_options(-std=c++20)
endif()

file(GLOB CPP_SOURCES "*.cpp")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)


foreach(source ${CPP_SOURCES})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    target_link_libraries(${name} Qt6::Core Qt6::Gui Qt6::Widgets)
endforeach()