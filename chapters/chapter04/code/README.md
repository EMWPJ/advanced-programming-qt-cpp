# 第4章代码示例

本目录包含第4章"编程范式详解"的所有代码示例，涵盖面向对象编程、泛型编程、函数式编程等多种编程范式。

## 📋 示例分类

### 🏗️ 面向对象编程 (OOP)
- `oop_example.cpp`: OOP基础示例
- `oop_example_code_reuse_*.cpp`: 代码复用（封装、继承、多态）
- `oop_example_complex_system.cpp`: 复杂系统设计
- `oop_example_gui.cpp`: GUI应用
- `oop_example_large_project.cpp`: 大型项目结构
- `oop_example_real_world.cpp`: 实际应用案例

### 🎯 泛型编程 (GP)
#### 函数模板
- `gp_function_template_*.cpp`: 函数模板示例（1-5）

#### 类模板
- `gp_class_template_*.cpp`: 类模板示例（1-10）

#### 模板元编程
- `gp_template_metaprogramming_*.cpp`: 模板元编程（1-9）

### 🔧 函数式编程 (FP)
- `functional_programming_*.cpp`: 函数式编程示例（0-9）

### 🧩 组件化编程
- `component_programming_*.cpp`: 组件化编程示例（1-3）

### 📝 声明式编程
- `declarative_programming.cpp`: 声明式编程示例

### ⚡ 事件驱动编程
- `event_driven_programming.cpp`: 事件驱动编程示例

### 🔄 过程式编程
- `process_example.cpp`: 过程式编程基础
- `process_example_*.cpp`: 过程式编程应用

## 🚀 构建运行

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 构建所有示例
cmake --build . --config Release

# 运行特定示例
./oop_example
```

## 📖 学习路径

### 阶段1: 编程范式基础
1. `process_example.cpp` - 过程式编程
2. `oop_example.cpp` - 面向对象编程
3. `gp_function_template_1.cpp` - 泛型编程基础
4. `functional_programming_0.cpp` - 函数式编程

### 阶段2: 深入学习
1. 完成各范式的系列示例
2. 对比不同范式的优缺点
3. 尝试范式混合使用

### 阶段3: 实践应用
1. `oop_example_large_project.cpp` - 大型项目设计
2. `component_programming_*.cpp` - 组件化开发
3. 综合应用多种范式

## 🎯 关键概念

- **封装**: 数据和行为的组合
- **继承**: 代码复用的层次结构
- **多态**: 运行时动态绑定
- **泛型**: 类型参数化
- **函数对象**: 可调用对象
- **模板特化**: 模板定制化

---
**示例维护**: 王培杰
**更新日期**: \today
