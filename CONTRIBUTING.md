# 贡献指南

感谢您对《高等程序设计 - Qt/C++》课程项目的兴趣！我们欢迎各种形式的贡献，包括但不限于：

- 🐛 报告和修复代码示例中的bug
- ✨ 添加新的代码示例和练习
- 📚 改进文档和注释
- 🎨 优化代码结构和可读性
- 🧪 添加测试用例
- 📖 完善README和文档

## 快速开始

### 1. Fork 项目

点击右上角的 "Fork" 按钮，将项目复制到您的GitHub账户中。

### 2. 克隆到本地

```bash
# 请将 YOUR_USERNAME 替换为实际的GitHub用户名
git clone https://github.com/YOUR_USERNAME/advanced-programming-qt-cpp.git
cd advanced-programming-qt-cpp
```

### 3. 创建特性分支

```bash
git checkout -b feature/your-feature-name
# 或者
git checkout -b fix/bug-description
```

## 开发规范

### 代码风格

我们遵循Qt编码规范和现代C++最佳实践：

#### C++ 代码规范
- 使用C++17标准特性
- 使用智能指针管理内存
- 遵循RAII原则
- 使用`const`正确性
- 避免全局变量
- 使用有意义的变量和函数名

#### 示例代码结构
```cpp
// 好的示例
#include <iostream>
#include <memory>

class Example {
public:
    explicit Example(int value) : m_value(value) {}

    void display() const {
        std::cout << "Value: " << m_value << std::endl;
    }

private:
    int m_value;
};

int main() {
    auto example = std::make_unique<Example>(42);
    example->display();
    return 0;
}
```

### 文件命名规范

- C++ 源文件：`snake_case.cpp`
- C++ 头文件：`snake_case.h`
- CMake 文件：`CMakeLists.txt`
- 文档文件：`README.md`

### 提交信息规范

提交信息应该清晰描述变更内容：

```bash
# 好的提交信息
git commit -m "feat: 添加向量容器的基本操作示例"

git commit -m "fix: 修复智能指针示例中的内存泄漏"

git commit -m "docs: 更新第2章的README说明"

git commit -m "refactor: 重构字符串处理函数，提高可读性"
```

### 分支命名

- 功能分支：`feature/feature-name`
- 修复分支：`fix/bug-description`
- 文档分支：`docs/update-documentation`
- 重构分支：`refactor/refactor-description`

## 贡献流程

### 1. 开发前准备

```bash
# 更新主分支
git checkout main
git pull origin main

# 创建新分支
git checkout -b feature/your-feature
```

### 2. 开发过程

- 编写代码并确保编译通过
- 添加必要的注释和文档
- 测试代码功能
- 遵循项目的代码规范

### 3. 提交变更

```bash
# 添加文件
git add .

# 提交（使用清晰的提交信息）
git commit -m "feat: 添加新的代码示例"

# 推送分支
git push origin feature/your-feature
```

### 4. 创建 Pull Request

1. 在GitHub上访问您的fork
2. 点击 "Compare & pull request"
3. 填写PR描述，包括：
   - 变更内容说明
   - 相关ISSUE编号
   - 测试说明
   - 截图（如果适用）

### 5. 代码审查

PR提交后，维护者会进行代码审查，可能要求：
- 修改代码风格
- 添加测试
- 更新文档
- 重构代码

## 代码示例要求

### 完整性
每个代码示例应该包含：
- `CMakeLists.txt` 构建文件
- 必要的头文件和源文件
- README.md 说明文档
- 示例输入输出

### CMake 配置示例

```cmake
cmake_minimum_required(VERSION 3.16)
project(example_name)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)
```

### 文档要求

每个示例目录应该包含README.md：

```markdown
# 示例名称

## 说明
简要描述这个示例演示的内容。

## 编译运行
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
./example_name
```

## 预期输出
```
Expected output here
```

## 学习要点
- 知识点1
- 知识点2
- 知识点3
```

## 测试要求

### 本地测试
在提交PR前，请确保：
- [ ] 代码能够正常编译
- [ ] 示例程序能够运行
- [ ] 输出结果符合预期
- [ ] 没有内存泄漏
- [ ] 代码符合规范

### CI/CD 测试
项目使用GitHub Actions进行自动化测试：
- 多平台编译测试（Windows、macOS、Linux）
- LaTeX文档编译测试
- 代码示例构建测试

## 问题反馈

如果您在贡献过程中遇到问题，可以：

1. 查看 [Issues](../../issues) 页面
2. 创建新的Issue描述问题
3. 在PR中请求帮助

## 许可证

通过贡献代码，您同意您的贡献将采用与项目相同的MIT许可证。

## 联系方式

如有任何疑问，请联系：
- **王培杰** (课程负责人)
- Email: 478451384@qq.com

感谢您的贡献！🎉
