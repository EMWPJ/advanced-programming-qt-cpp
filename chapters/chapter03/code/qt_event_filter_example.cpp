#include <QWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QApplication>

// 自定义事件过滤器
class MyEventFilter : public QObject {
    Q_OBJECT
public:
    explicit MyEventFilter(QObject *parent = nullptr) : QObject(parent) {}
protected:
    bool eventFilter(QObject *, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            qDebug() << "过滤器: 按键:" << QKeySequence(keyEvent->key()).toString();
            if (keyEvent->key() == Qt::Key_Escape) {
                qDebug() << "过滤器: 拦截Esc";
                return true;
            }
        }
        return false;
    }
};

// 事件过滤器控件
class EventFilterWidget : public QWidget {
    Q_OBJECT
public:
    EventFilterWidget(QWidget *parent = nullptr) : QWidget(parent) {
        installEventFilter(new MyEventFilter(this));
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    void keyPressEvent(QKeyEvent *event) override {
        qDebug() << "收到按键: " << QKeySequence(event->key()).toString();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventFilterWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_event_filter_example.moc"