# 第3章代码示例

本目录包含第3章"Qt框架基础知识"的所有代码示例，涵盖Qt核心概念、信号与槽、控件使用、事件处理等。

## 📋 示例分类

### 🎯 核心概念
- `qobject_example.*`: QObject基类和对象树
- `qt_signal_slot_example.cpp`: 信号与槽基础
- `qt_connect_example.cpp`: 连接方式详解
- `qt_connect_advanced_example.cpp`: 高级连接技巧

### 🎨 界面控件
- `qt_widget_example.cpp`: 基础控件使用
- `qt_pushbutton_example.cpp`: 按钮控件
- `qt_label_example.cpp`: 标签控件
- `qt_lineedit_example.cpp`: 单行文本框
- `qt_textedit_example.cpp`: 多行文本编辑器
- `qt_checkbox_example.cpp`: 复选框
- `qt_radiobutton_example.cpp`: 单选按钮
- `qt_combobox_example.cpp`: 下拉列表
- `qt_listwidget_example.cpp`: 列表控件
- `qt_treewidget_example.cpp`: 树形控件

### 🏗️ 窗口和布局
- `qt_mainwindow_example.cpp`: 主窗口
- `qt_menubar_example.cpp`: 菜单栏
- `qt_toolbar_example.cpp`: 工具栏
- `qt_statusbar_example.cpp`: 状态栏
- `qt_tabwidget_example.cpp`: 选项卡
- `qt_stackedwidget_example.cpp`: 堆叠控件
- `qt_splitter_example.cpp`: 分割器
- `qt_scrollarea_example.cpp`: 滚动区域

### 🎭 对话框和容器
- `qt_dialog_example.cpp`: 对话框
- `qt_frame_example.cpp`: 框架控件

### ⚡ 事件处理
- `qt_event_example.cpp`: 事件基础
- `qt_event_handler_example.cpp`: 事件处理器
- `qt_event_filter_example.cpp`: 事件过滤器
- `qt_mouse_event_example.cpp`: 鼠标事件
- `qt_keyboard_event_example.cpp`: 键盘事件
- `qt_timer_event_example.cpp`: 定时器事件
- `qt_custom_event_example.cpp`: 自定义事件
- `qt_drag_drop_event_example.cpp`: 拖放事件

### 📊 数据结构
- `qt_list_example.cpp`: QList使用
- `qt_string_list_example.cpp`: 字符串列表
- `qt_map_example.cpp`: QMap使用
- `qt_hash_example.cpp`: QHash使用
- `qt_set_example.cpp`: QSet使用
- `qt_stack_example.cpp`: QStack使用
- `qt_queue_example.cpp`: QQueue使用

### 🔧 系统功能
- `qt_file_example.cpp`: 文件操作
- `qt_fileinfo_example.cpp`: 文件信息
- `qt_dir_example.cpp`: 目录操作
- `qt_process_example.cpp`: 进程管理
- `qt_settings_example.cpp`: 设置存储
- `qt_timer_example.cpp`: 定时器
- `qt_thread_example.cpp`: 多线程

### 🎨 界面美化
- `qt_qss_example.cpp`: Qt样式表
- `qt_painter_example.cpp`: 绘图

### 📅 日期时间
- `qt_datetime_example.cpp`: 日期时间
- `qt_datetimeedit_example.cpp`: 日期时间编辑器
- `qt_calendarwidget_example.cpp`: 日历控件

### 🔢 数值控件
- `qt_spinbox_example.cpp`: 数值调节框
- `qt_doublespinbox_example.cpp`: 双精度数值框
- `qt_slider_example.cpp`: 滑块
- `qt_progressbar_example.cpp`: 进度条
- `qt_scrollbar_example.cpp`: 滚动条

### 📄 数据流
- `qt_datastream_example.cpp`: 数据流
- `qt_textstream_example.cpp`: 文本流
- `qt_variant_example.cpp`: QVariant使用
- `qt_bytearray_example.cpp`: 字节数组

### 🔄 并发编程
- `qt_concurrent_example.cpp`: Qt并发

## 🚀 构建运行

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 构建所有示例
cmake --build . --config Release

# 运行特定示例
./qt_widget_example
```

## 📖 学习建议

1. **循序渐进**: 从基础控件开始，逐步学习高级特性
2. **实践为主**: 每个示例都动手运行和修改
3. **信号与槽**: 重点掌握Qt的事件处理机制
4. **布局管理**: 学习创建美观的界面布局

## 🎯 关键概念

- **信号与槽**: Qt的事件处理机制
- **对象树**: Qt的内存管理模型
- **元对象系统**: MOC和反射机制
- **事件循环**: Qt应用程序的核心

---
**示例维护**: 王培杰
**更新日期**: \today
