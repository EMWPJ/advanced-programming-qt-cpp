#include <QApplication>  // Qt应用程序主类
#include <QWidget>       // Qt窗口部件基类
#include <QDebug>        // Qt调试输出类

int main(int argc, char *argv[])
{
    // 创建QApplication对象，必须在任何Qt窗口部件之前创建
    // 在调试时，可以在此处查看argc/argv的值，了解程序启动参数
    QApplication app(argc, argv);
    // 创建一个QWidget窗口对象
    // 【调试建议】可在此行设置断点，调试时程序会在此暂停，便于单步跟踪后续窗口属性设置
    QWidget window;
    // 设置窗口标题
    // 在调试时，可以检查window对象的属性是否已正确设置
    window.setWindowTitle("调试示例");
    // 设置窗口大小
    window.resize(400, 300);
    // 输出调试信息到控制台
    // 【调试建议】可在调试器中观察此输出，确认窗口已成功创建
    qDebug() << "窗口创建成功";
    // 显示窗口
    window.show();
    // 进入Qt事件循环，等待用户操作
    // 【调试建议】可在此处监视result变量，查看应用程序的退出码
    int result = app.exec();
    // 输出应用程序退出时的返回值
    // 便于调试时确认程序是否正常退出
    qDebug() << "应用程序以返回值退出:" << result;
    // 返回应用程序的退出码
    return result;
}
