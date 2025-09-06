#include <QWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QApplication>

// 事件处理器控件
class EventHandlerWidget : public QWidget
{
    Q_OBJECT
public:
    EventHandlerWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFocusPolicy(Qt::StrongFocus);
    }
protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            qDebug() << "事件处理器: 按键:" << QKeySequence(keyEvent->key()).toString();
            if (keyEvent->key() == Qt::Key_Escape) {
                qDebug() << "事件处理器: 拦截Esc";
                return true;
            }
        }
        return QWidget::event(event);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventHandlerWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_event_handler_example.moc"