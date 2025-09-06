# 第5章代码示例

本目录包含第5章"设计模式"的所有代码示例，涵盖23种经典设计模式的C++实现。

## 📋 设计模式分类

### 🏭 创建型模式 (Creational Patterns)
- `singleton_pattern.cpp`: 单例模式 - 确保类只有一个实例
- `factory_method_pattern.cpp`: 工厂方法 - 创建对象的方法
- `abstract_factory_pattern.cpp`: 抽象工厂 - 创建相关对象族
- `builder_pattern.cpp`: 建造者模式 - 分步骤构建复杂对象
- `prototype_pattern.cpp`: 原型模式 - 通过复制创建对象

### 🏗️ 结构型模式 (Structural Patterns)
- `adapter_pattern.cpp`: 适配器模式 - 接口转换
- `bridge_pattern.cpp`: 桥接模式 - 抽象与实现分离
- `composite_pattern.cpp`: 组合模式 - 对象树结构
- `decorator_pattern.cpp`: 装饰器模式 - 动态添加功能
- `facade_pattern.cpp`: 外观模式 - 简化接口
- `flyweight_pattern.cpp`: 享元模式 - 共享对象
- `proxy_pattern.cpp`: 代理模式 - 控制访问

### ⚡ 行为型模式 (Behavioral Patterns)
- `chain_of_responsibility_pattern.cpp`: 责任链 - 请求传递
- `command_pattern.cpp`: 命令模式 - 请求封装
- `interpreter_pattern.cpp`: 解释器模式 - 语言解释
- `iterator_pattern.cpp`: 迭代器模式 - 集合遍历
- `mediator_pattern.cpp`: 中介者模式 - 对象交互
- `memento_pattern.cpp`: 备忘录模式 - 状态保存
- `observer_pattern.cpp`: 观察者模式 - 状态通知
- `state_pattern.cpp`: 状态模式 - 对象状态
- `strategy_pattern.cpp`: 策略模式 - 算法封装
- `template_method_pattern.cpp`: 模板方法 - 算法框架
- `visitor_pattern.cpp`: 访问者模式 - 对象操作

## 🚀 构建运行

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 构建所有示例
cmake --build . --config Release

# 运行特定模式示例
./singleton_pattern
```

### 批量运行脚本

```bash
# Windows
./run.bat

# 手动运行
for pattern in *_pattern.cpp; do
    exe="${pattern%.cpp}"
    g++ -std=c++17 "$pattern" -o "$exe"
    ./$exe
done
```

## 📖 学习建议

1. **按类型学习**: 先掌握每种类型的模式特点
2. **理解意图**: 明白每个模式要解决什么问题
3. **对比学习**: 比较相似模式的区别
4. **实践应用**: 在实际项目中应用设计模式

## 🎯 关键概念

- **设计原则**: SOLID原则、开闭原则等
- **模式意图**: 每个模式要解决的核心问题
- **结构组成**: 模式的类图和参与者
- **适用场景**: 什么情况下使用该模式

## 📊 模式复杂度

### 简单模式 ⭐
- Singleton, Factory Method, Adapter, Decorator, Observer

### 中等模式 ⭐⭐
- Builder, Prototype, Bridge, Composite, Strategy, Command

### 复杂模式 ⭐⭐⭐
- Abstract Factory, Template Method, Visitor, Interpreter

## 🔄 模式关系

- **相关模式**: 功能相似但解决不同问题的模式
- **替代模式**: 可以互相替代的模式
- **组合使用**: 多个模式一起使用的场景

---
**示例维护**: 王培杰
**更新日期**: \today