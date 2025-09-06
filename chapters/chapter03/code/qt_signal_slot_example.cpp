#include <QCoreApplication>
#include <QObject>
#include <QDebug>
// 定义一个按钮类，继承自QObject，支持信号槽
class Button : public QObject
{
    Q_OBJECT
public:
    // 模拟点击按钮的动作
    void click() {
        qDebug() << "按钮: 正在被点击...";
        emit clicked();
    }
signals:
    void clicked();
};

// 定义一个槽函数的接收者
class Receiver : public QObject
{
    Q_OBJECT
public slots:
    void onButtonClicked() {
        qDebug() << "按钮被点击了！";
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Button btn;
    Receiver receiver;
    // 信号与槽连接
    QObject::connect(&btn, &Button::clicked, &receiver, &Receiver::onButtonClicked);
    // 模拟用户点击
    btn.click();
    return app.exec();
}

#include "qt_signal_slot_example.moc"