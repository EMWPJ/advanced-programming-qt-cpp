#include <QWidget>
#include <QTimerEvent>
#include <QDebug>
#include <QPainter>
#include <QApplication>
class TimerEventWidget : public QWidget
{
    Q_OBJECT
public:
    TimerEventWidget(QWidget *parent = nullptr) : QWidget(parent), m_counter(0) {
        // 启动定时器，间隔1000毫秒（1秒）
        m_timerId = startTimer(1000);
    }
protected:
    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() == m_timerId) {
            m_counter++;
            qDebug() << "定时器事件触发，计数:" << m_counter;
            update();
        }
    }
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(Qt::darkGreen);
        painter.drawText(rect(), Qt::AlignCenter, QString("定时器计数: %1").arg(m_counter));
    }
private:
    int m_timerId;
    int m_counter;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TimerEventWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_timer_event_example.moc"