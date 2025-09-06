#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QPushButton>
// 定义自定义事件类型
const QEvent::Type MyCustomEventType = static_cast<QEvent::Type>(QEvent::User + 1);
class MyCustomEvent : public QEvent
{
public:
    MyCustomEvent(const QString& msg)
        : QEvent(MyCustomEventType), m_message(msg) {}

    QString message() const { return m_message; }
private:
    QString m_message;
};

class CustomEventWidget : public QWidget
{
    Q_OBJECT
public:
    CustomEventWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QPushButton *btn = new QPushButton("发送自定义事件", this);
        btn->setGeometry(100, 80, 200, 40);
        connect(btn, &QPushButton::clicked, this, [this]() {
            // 发送自定义事件
            QCoreApplication::postEvent(this, new MyCustomEvent("Hello, 这是自定义事件!"));
        });
    }
protected:
    // 事件处理
    void customEvent(QEvent *event) override
    {
        if (event->type() == MyCustomEventType) {
            MyCustomEvent *myEvent = static_cast<MyCustomEvent*>(event);
            qDebug() << "收到自定义事件，消息内容:" << myEvent->message();
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    CustomEventWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_custom_event_example.moc"