#include <QApplication> // Qt应用程序类
#include <QMainWindow> // Qt主窗口类
#include <QLabel> // Qt标签类
#include <QVBoxLayout> // Qt垂直布局类
#include <QWidget> // Qt窗口部件类

int main(int argc, char *argv[]) // 主函数
{
    // 创建应用程序对象
    QApplication app(argc, argv);

    // 创建主窗口
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Qt Application Example");
    mainWindow.resize(400, 300);

    // 创建中心部件
    QWidget *centralWidget = new QWidget(&mainWindow);
    // 设置主窗口的中心部件
    mainWindow.setCentralWidget(centralWidget);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // 创建标签
    QLabel *label = new QLabel("Hello, Qt!", centralWidget);
    // 设置标签的文本对齐方式
    label->setAlignment(Qt::AlignCenter);
    // 将标签添加到布局中
    layout->addWidget(label);

    // 显示窗口
    mainWindow.show();

    // 进入事件循环
    return app.exec();
}