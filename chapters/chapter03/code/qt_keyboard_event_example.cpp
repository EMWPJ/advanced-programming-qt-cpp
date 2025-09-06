#include <QWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QApplication>
#include <QSet>

class KeyboardEventWidget : public QWidget
{
    Q_OBJECT
public:
    KeyboardEventWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFocusPolicy(Qt::StrongFocus); // 允许接收键盘焦点
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        m_pressedKeys.insert(event->key());
        // 组合键示例：Ctrl + S
        if (m_pressedKeys.contains(Qt::Key_Control) && m_pressedKeys.contains(Qt::Key_S)) {
            qDebug() << "检测到组合键：Ctrl + S";
        }
        // 组合键示例：Shift + A
        else if (m_pressedKeys.contains(Qt::Key_Shift) && m_pressedKeys.contains(Qt::Key_A)) {
            qDebug() << "检测到组合键：Shift + A";
        }
        // 只按下单个键
        else {
            qDebug() << "按下键：" << QKeySequence(event->key()).toString() << "(key code:" << event->key() << ")";
        }
        update();
    }
    void keyReleaseEvent(QKeyEvent *event) override {
        m_pressedKeys.remove(event->key());
        qDebug() << "释放键：" << QKeySequence(event->key()).toString() << "(key code:" << event->key() << ")";
    }
private:
    QSet<int> m_pressedKeys;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KeyboardEventWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_keyboard_event_example.moc"