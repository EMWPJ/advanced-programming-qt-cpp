# 第1章图片资源

本目录包含第1章"开发环境搭建"课件中使用的图片资源。

## 📋 图片列表

- `qt_logo.png`: Qt官方logo，用于课件品牌展示

## 🎨 图片规范

### 文件格式
- **PNG**: 透明背景，适合logo和图标
- **SVG**: 矢量格式，适合流程图（推荐）
- **PDF**: 高质量矢量格式

### 命名规范
- 使用小写字母和下划线
- 描述性名称，如 `qt_logo.png`
- 避免中文文件名

### 图片要求
- **分辨率**: 适合演示的清晰度
- **大小**: 控制在合理范围内（< 1MB）
- **格式**: 支持LaTeX包含的格式

## 🛠️ LaTeX使用

```latex
% 包含图片
\includegraphics[width=0.8\textwidth]{images/qt_logo.png}

% 图片环境
\begin{figure}[H]
    \centering
    \includegraphics[height=2cm]{images/qt_logo.png}
    \caption{Qt官方logo}
    \label{fig:qt_logo}
\end{figure}
```

## 📖 使用说明

1. **相对路径**: 使用相对路径引用图片
2. **编译要求**: 确保图片文件存在于正确位置
3. **格式转换**: 如需要可使用工具转换格式

## 🎯 最佳实践

- 图片文件名使用英文
- 为图片添加有意义的caption
- 使用合适的图片大小
- 注意版权和授权

---
**资源维护**: 王培杰
**更新日期**: \today
