#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QTimer>

// 事件循环演示控件
class EventLoopWidget : public QWidget
{
    Q_OBJECT
public:
    EventLoopWidget(QWidget *parent = nullptr) : QWidget(parent), m_counter(0)
    {
        // 启动定时器，每秒触发一次
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &EventLoopWidget::onTimeout);
        timer->start(1000);
    }

protected:
    // 重写事件处理函数，演示事件循环分发
    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::Timer) {
            qDebug() << "收到定时器事件，计数:" << m_counter;
        }
        return QWidget::event(event);
    }

private slots:
    void onTimeout()
    {
        m_counter++;
        qDebug() << "定时器超时，发送自定义事件，计数:" << m_counter;
        // 这里可以发送自定义事件或做其他操作
    }

private:
    int m_counter;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventLoopWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_event_loop_example.moc"