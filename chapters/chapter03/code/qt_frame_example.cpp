#include <QApplication>
#include <QFrame>
#include <QDebug>
class FrameDemo : public QFrame
{
public:
    FrameDemo(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QFrame(parent, f)
    {
        setWindowTitle("QFrame 演示");
        resize(400, 250);
        setFrameShape(QFrame::Box); // 设置边框样式
        // QFrame::Box 边框样式
        // QFrame::NoFrame 无边框
        // QFrame::Panel 面板
        // QFrame::StyledPanel 样式化面板
        // QFrame::HLine 水平线
        // QFrame::VLine 垂直线
        // QFrame::WinPanel Windows 2000风格面板

        setFrameShadow(QFrame::Raised); // 设置边框阴影
        // QFrame::Plain 无阴影
        // QFrame::Raised 凸起
        // QFrame::Sunken 凹陷

        setLineWidth(3); // 设置边框宽度
        setMidLineWidth(1); // 设置中间线宽度
        qDebug() << "frameRect():" << frameRect();
        qDebug() << "frameShadow():" << frameShadow();
        qDebug() << "frameShape():" << frameShape();
        qDebug() << "frameWidth():" << frameWidth();
        qDebug() << "lineWidth():" << lineWidth();
        qDebug() << "midLineWidth():" << midLineWidth();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FrameDemo frame;
    frame.show();
    return app.exec();
}
