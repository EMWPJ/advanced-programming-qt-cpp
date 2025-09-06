# 第5章图片资源

本目录包含第5章"设计模式"课件中使用的图片资源，包括所有23种经典设计模式的结构图。

## 📋 图片列表

### 创建型模式 (Creational Patterns)
- `singleton_pattern.*`: 单例模式类图
- `factory_method_pattern.*`: 工厂方法模式类图
- `abstract_factory_pattern.*`: 抽象工厂模式类图
- `builder_pattern.*`: 建造者模式类图
- `prototype_pattern.*`: 原型模式类图

### 结构型模式 (Structural Patterns)
- `adapter_pattern.*`: 适配器模式类图
- `bridge_pattern.*`: 桥接模式类图
- `composite_pattern.*`: 组合模式类图
- `decorator_pattern.*`: 装饰器模式类图
- `facade_pattern.*`: 外观模式类图
- `flyweight_pattern.*`: 享元模式类图
- `proxy_pattern.*`: 代理模式类图

### 行为型模式 (Behavioral Patterns)
- `chain_of_responsibility_pattern.*`: 责任链模式类图
- `command_pattern.*`: 命令模式类图
- `interpreter_pattern.*`: 解释器模式类图
- `iterator_pattern.*`: 迭代器模式类图
- `mediator_pattern.*`: 中介者模式类图
- `memento_pattern.*`: 备忘录模式类图
- `observer_pattern.*`: 观察者模式类图
- `state_pattern.*`: 状态模式类图
- `strategy_pattern.*`: 策略模式类图
- `template_method_pattern.*`: 模板方法模式类图
- `visitor_pattern.*`: 访问者模式类图

## 🎨 文件格式

### Graphviz DOT文件 (.dot)
- 矢量图源文件
- 可使用Graphviz工具编辑
- 生成命令：`dot -Tpdf file.dot -o file.pdf`

### PDF文件 (.pdf)
- LaTeX兼容的矢量格式
- 适合高质量打印
- 编译时直接包含

### 批量生成脚本
- `generate_pdfs.bat`: Windows批处理脚本
- 用于重新生成所有PDF文件

## 🛠️ 使用方法

### LaTeX中包含图片
```latex
\begin{figure}[H]
    \centering
    \includegraphics[width=0.9\textwidth]{images/singleton_pattern.pdf}
    \caption{单例模式类图}
    \label{fig:singleton_pattern}
\end{figure}
```

### 编辑图片
```bash
# 安装Graphviz
# Ubuntu/Debian
sudo apt-get install graphviz

# 编辑DOT文件
dot -Tpdf singleton_pattern.dot -o singleton_pattern.pdf
```

## 📖 图表说明

每个设计模式图表包含：
- **类结构**: 抽象类、具体类、接口
- **关系**: 继承、实现、依赖、关联
- **参与者**: 模式中的各个角色
- **交互**: 类之间的协作关系

## 🎯 图表规范

- **统一风格**: 所有图表使用一致的样式
- **清晰标注**: 类名、方法名清晰可读
- **关系明确**: 不同关系使用不同线型
- **布局合理**: 图表结构层次分明

## 🔧 维护说明

### 更新图表
1. 编辑对应的`.dot`文件
2. 运行`generate_pdfs.bat`重新生成PDF
3. 提交更改到版本控制

### 添加新模式
1. 创建新的`.dot`文件
2. 遵循现有命名规范
3. 更新`generate_pdfs.bat`脚本

---
**资源维护**: 王培杰
**更新日期**: \today
