#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

class MouseEventWidget : public QWidget
{
    Q_OBJECT
public:
    MouseEventWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMouseTracking(true); // 启用鼠标跟踪
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        qDebug() << "鼠标按下，位置:" << event->pos();
        m_lastPos = event->pos();
        update();
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        qDebug() << "鼠标移动，位置:" << event->pos();
        m_lastPos = event->pos();
        update();
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        qDebug() << "鼠标释放，位置:" << event->pos();
        m_lastPos = event->pos();
        update();
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawEllipse(m_lastPos, 5, 5);
    }

private:
    QPoint m_lastPos;
};

// main函数示例
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MouseEventWidget w;
    w.resize(300, 200);
    w.show();
    return app.exec();
}

#include "qt_mouse_event_example.moc"