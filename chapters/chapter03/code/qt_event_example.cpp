#include <QCoreApplication>
#include <QEvent>
#include <QObject>
#include <QDebug>
// 自定义一个按钮点击事件类型
const QEvent::Type ButtonClickEventType = static_cast<QEvent::Type>(QEvent::User + 1);
// 自定义事件类
class ButtonClickEvent : public QEvent {
public:
    ButtonClickEvent() : QEvent(ButtonClickEventType) {}
};
// 按钮类，继承自QObject
class Button : public QObject {
    Q_OBJECT
public:
    // 发送点击事件
    void click() {
        qDebug() << "按钮: 正在被点击...";
        // 创建事件对象
        ButtonClickEvent event;
        // 发送事件到自身
        QCoreApplication::sendEvent(this, &event);
    }
protected:
    // 重写事件处理函数
    bool event(QEvent *e) override {
        if (e->type() == ButtonClickEventType) {
            qDebug() << "按钮被点击了！（事件处理）";
            return true;
        }
        return QObject::event(e);
    }
};
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    Button btn;
    // 模拟用户点击
    btn.click();
    return app.exec();
}

#include "qt_event_example.moc"