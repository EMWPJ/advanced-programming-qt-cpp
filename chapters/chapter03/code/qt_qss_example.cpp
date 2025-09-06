#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("QSS 样式表示例");
    // 创建控件
    QLabel *label = new QLabel("欢迎使用 QSS 美化界面");
    QPushButton *btn1 = new QPushButton("普通按钮");
    QPushButton *btn2 = new QPushButton("高亮按钮");
    btn2->setObjectName("btn2"); // 优化：提前设置 objectName
    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(btn1);
    layout->addWidget(btn2);
    layout->setSpacing(16); // 优化：增加控件间距
    layout->setContentsMargins(24, 24, 24, 24); // 优化：设置窗口边距
    window.setLayout(layout);
    // 优化：QSS 样式结构更清晰，注释更明确
    QString qss = R"(
        /* 窗口背景 */
        QWidget {
            background-color: #f0f0f0;
        }
        /* 标签样式 */
        QLabel {
            color: #2d8cf0;
            font-size: 20px;
            font-weight: bold;
            padding: 8px;
        }
        /* 普通按钮样式 */
        QPushButton {
            background-color: #ffffff;
            border: 2px solid #2d8cf0;
            border-radius: 8px;
            color: #2d8cf0;
            font-size: 16px;
            padding: 6px 18px;
            min-width: 100px;
            min-height: 32px;
            transition: all 0.2s;
        }
        QPushButton:hover {
            background-color: #2d8cf0;
            color: #ffffff;
        }
        QPushButton:pressed {
            background-color: #1565c0;
            color: #ffffff;
        }
        /* 高亮按钮样式 */
        QPushButton#btn2 {
            background-color: #ff9800;
            color: #fff;
            border: 2px solid #ff9800;
        }
        QPushButton#btn2:hover {
            background-color: #ffa726;
            border-color: #ffa726;
        }
        QPushButton#btn2:pressed {
            background-color: #ef6c00;
            border-color: #ef6c00;
        }
    )";
    window.setStyleSheet(qss);
    window.resize(360, 220); // 优化：更合适的窗口大小
    window.show();
    return app.exec();
}
