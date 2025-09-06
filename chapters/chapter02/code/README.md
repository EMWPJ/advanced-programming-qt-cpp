# 第2章代码示例

本目录包含第2章"C++语言基础与进阶"的所有代码示例，涵盖C语言基础、C++面向对象编程、模板和现代C++特性。

## 📋 示例分类

### 🔵 C语言基础示例

#### 基本语法
- `c_basic_syntax.c`: C语言基础语法（变量、数据类型、运算符）
- `c_control_structures.c`: 控制结构（if-else、循环、switch）

#### 函数与指针
- `c_function_simple.c`: 简单函数定义和调用
- `c_function_example.c`: 函数参数传递和返回值
- `c_pointer_example.c`: 指针基础使用
- `c_pointer_memory_example.c`: 动态内存分配

#### 数据结构
- `c_struct_example.c`: 结构体定义和使用

### 🟢 C++基础示例

#### 基本特性
- `cpp_basic_syntax.cpp`: C++基础语法
- `cpp_basic_types.cpp`: C++基本数据类型
- `cpp_basic_types_demo.cpp`: 数据类型演示
- `cpp_variables_constants.cpp`: 变量和常量
- `cpp_variables_constants_clean.cpp`: 清理版变量常量示例

#### 字符串和数组
- `cpp_string_example.cpp`: 字符串处理
- `cpp_array_example.cpp`: 数组操作
- `cpp_stdarray_example.cpp`: std::array使用
- `cpp_vector_example.cpp`: STL向量容器

#### 控制结构
- `cpp_control_structures.cpp`: C++控制结构
- `cpp_range_for.cpp`: 范围for循环

### 🟡 面向对象编程

#### 类和对象
- `cpp_class_example.cpp`: 类定义和对象创建
- `cpp_const_correctness.cpp`: const正确性

#### 继承与多态
- `cpp_inherit_example.cpp`: 单继承示例
- `cpp_inherit_rule.cpp`: 继承规则
- `cpp_multi_inherit_example.cpp`: 多重继承
- `cpp_polymorphism_example.cpp`: 多态实现

### 🟠 高级特性

#### 函数特性
- `cpp_function_basic.cpp`: 函数基础
- `cpp_function_overload.cpp`: 函数重载

#### 指针与引用
- `cpp_pointer_example.cpp`: C++指针
- `cpp_reference_example.cpp`: 引用使用

#### 现代C++特性
- `cpp_auto.cpp`: auto类型推导
- `cpp_auto_type_deduction.cpp`: 类型推导演示
- `cpp_lambda_example.cpp`: Lambda表达式
- `cpp_move_semantics.cpp`: 移动语义
- `cpp_operators.cpp`: 运算符重载

#### 智能指针
- `cpp_smart_pointer.cpp`: 智能指针基础
- `cpp_smart_pointer_clean.cpp`: 智能指针清理版
- `cpp_smart_pointers_demo.cpp`: 智能指针演示

#### Qt集成
- `cpp_qt_example.cpp`: Qt与C++集成示例

## 🚀 构建和运行

### 使用CMake构建（推荐）

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 构建所有示例
cmake --build . --config Release
```

### 手动编译示例

```bash
# 编译C程序
gcc c_basic_syntax.c -o c_basic_syntax
./c_basic_syntax

# 编译C++程序
g++ -std=c++17 cpp_basic_syntax.cpp -o cpp_basic_syntax
./cpp_basic_syntax

# 编译Qt程序
g++ -std=c++17 cpp_qt_example.cpp -o qt_example \
    $(pkg-config --cflags --libs Qt6Core Qt6Widgets)
./qt_example
```

### 批量编译脚本

```bash
# Linux/macOS
for file in *.c *.cpp; do
    if [[ $file == *.c ]]; then
        gcc "$file" -o "${file%.c}"
    else
        g++ -std=c++17 "$file" -o "${file%.cpp}"
    fi
done

# Windows PowerShell
Get-ChildItem *.c,*.cpp | ForEach-Object {
    $output = $_.BaseName
    if ($_.Extension -eq '.c') {
        gcc $_.Name -o $output
    } else {
        g++ -std=c++17 $_.Name -o $output
    }
}
```

## 📖 学习路径

### 阶段1: C语言基础 (推荐学习顺序)
1. `c_basic_syntax.c` - 掌握基础语法
2. `c_control_structures.c` - 学习控制流
3. `c_function_simple.c` → `c_function_example.c` - 函数编程
4. `c_pointer_example.c` → `c_pointer_memory_example.c` - 指针和内存
5. `c_struct_example.c` - 数据结构

### 阶段2: C++基础
1. `cpp_basic_syntax.cpp` - C++语法基础
2. `cpp_variables_constants.cpp` - 变量和常量
3. `cpp_string_example.cpp` - 字符串处理
4. `cpp_array_example.cpp` → `cpp_vector_example.cpp` - 容器使用

### 阶段3: 面向对象编程
1. `cpp_class_example.cpp` - 类和对象
2. `cpp_inherit_example.cpp` - 继承
3. `cpp_polymorphism_example.cpp` - 多态

### 阶段4: 高级特性
1. `cpp_lambda_example.cpp` - 函数式编程
2. `cpp_smart_pointer.cpp` - 现代内存管理
3. `cpp_move_semantics.cpp` - 性能优化
4. `cpp_qt_example.cpp` - Qt集成

## 🎯 关键概念演示

### 1. 内存管理对比
```cpp
// C风格
int* ptr = (int*)malloc(sizeof(int));
// 使用后必须手动释放
free(ptr);

// C++现代方式
auto ptr = std::make_unique<int>(42);
// 自动释放，无需手动管理
```

### 2. 字符串处理对比
```cpp
// C风格
char str[] = "Hello";
// 容易出错，需小心缓冲区

// C++方式
std::string str = "Hello";
// 类型安全，自动管理内存
```

### 3. 多态实现
```cpp
class Animal {
public:
    virtual void speak() = 0;  // 纯虚函数
};

class Dog : public Animal {
public:
    void speak() override { std::cout << "Woof!"; }
};
```

## 🚨 常见问题

### 编译错误
- **C++17支持**: 确保编译器支持C++17 (`-std=c++17`)
- **头文件路径**: 检查标准库头文件路径
- **链接错误**: 确认链接了所有必要的库

### 运行时错误
- **段错误**: 检查指针使用是否正确
- **内存泄漏**: 使用智能指针或Valgrind检查
- **未定义行为**: 避免数组越界和野指针

### 调试技巧
- 使用`-g`编译选项启用调试信息
- 使用gdb或lldb进行调试
- 添加打印语句跟踪程序执行
- 使用assert进行运行时检查

## 🔧 开发环境

### 推荐工具
- **编译器**: GCC 9+/Clang 7+/MSVC 2019+
- **构建工具**: CMake 3.16+
- **调试器**: gdb/lldb
- **IDE**: VS Code + C++扩展 / Qt Creator

### 环境配置
```bash
# 检查编译器版本
g++ --version

# 设置C++17标准
export CXXFLAGS="-std=c++17 -Wall -Wextra"

# 启用调试信息
export CXXFLAGS="$CXXFLAGS -g -O0"
```

## 📈 扩展练习

1. **性能对比**: 比较C和C++不同实现的性能
2. **错误处理**: 添加异常处理和错误恢复
3. **单元测试**: 为示例代码编写测试用例
4. **代码重构**: 将C风格代码重构为现代C++
5. **项目实践**: 将多个示例整合成完整程序

## 📚 参考资源

- [C++参考手册](https://en.cppreference.com/)
- [C++核心指南](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Qt文档](https://doc.qt.io/)
- [STL参考](https://www.cplusplus.com/reference/)

## 📞 技术支持

如有疑问，请：
1. 查看编译器错误信息
2. 参考示例代码注释
3. 查看C++标准文档
4. 联系课程助教

---
**示例维护**: 王培杰
**更新日期**: \today
