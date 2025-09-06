#include <QWidget>
#include <QEvent>
#include <QDebug>
#include <QApplication>
#include <QPainter>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QPaintEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QCloseEvent>
class WindowEventWidget : public QWidget
{
    Q_OBJECT
public:
    WindowEventWidget(QWidget *parent = nullptr) : QWidget(parent) {}
protected:
    void showEvent(QShowEvent *event) override {
        qDebug() << "窗口显示事件";
        QWidget::showEvent(event);
    }
    void hideEvent(QHideEvent *event) override {
        qDebug() << "窗口隐藏事件";
        QWidget::hideEvent(event);
    }

    void closeEvent(QCloseEvent *event) override {
        qDebug() << "窗口关闭事件";
        QWidget::closeEvent(event);
    }
    void resizeEvent(QResizeEvent *event) override {
        qDebug() << "窗口大小改变:" << event->size();
        QWidget::resizeEvent(event);
    }
    void moveEvent(QMoveEvent *event) override {
        qDebug() << "窗口移动到:" << event->pos();
        QWidget::moveEvent(event);
    }
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setPen(Qt::red);
        painter.drawText(rect(), Qt::AlignCenter, QString("窗口绘制事件"));
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WindowEventWidget w;
    w.resize(400, 300);
    w.show();
    return app.exec();
}

#include "qt_window_event_example.moc"