// 预处理指令示例
#define MAX 100                // 定义常量宏，将MAX替换为100
#define SQUARE(x) ((x)*(x))    // 带参数宏，SQUARE(5)替换为((5)*(5))
#include <stdio.h>             // 包含标准头文件
#include "myheader.h"          // 包含自定义头文件
#ifdef DEBUG                   // 如果定义了DEBUG宏，则编译以下内容
    #ifndef WIN32              // 如果未定义WIN32宏，则编译以下内容
        #if VERSION > 2        // 如果VERSION大于2，则编译以下内容
            // 这里可以放需要条件编译的代码
        #else                  // 如果条件不满足，则编译以下内容
            // 这里可以放条件不满足时的代码
        #endif                 // 结束VERSION条件编译
    #endif                     // 结束WIN32条件编译
#endif                         // 结束DEBUG条件编译
#undef MAX                     // 取消MAX宏定义
#pragma once                   // 防止头文件重复包含（仅限头文件使用）
#error "Unsupported platform"  // 生成编译错误
#warning "Deprecated function" // 生成编译警告