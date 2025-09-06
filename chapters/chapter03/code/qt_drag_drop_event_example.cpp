#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QPainter>
#include <QApplication>

class DragDropWidget : public QWidget
{
    Q_OBJECT
public:
    DragDropWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setAcceptDrops(true); // 启用拖放
        m_text = "请拖拽文本到此窗口";
    }
protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        // 判断拖入的数据类型是否为文本
        if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
            qDebug() << "拖拽进入，数据类型为文本";
        } else {
            event->ignore();
        }
    }
    void dropEvent(QDropEvent *event) override {
        if (event->mimeData()->hasText()) {
            m_text = event->mimeData()->text();
            qDebug() << "拖拽释放，接收到文本:" << m_text;
            event->acceptProposedAction();
            update();
        } else {
            event->ignore();
        }
    }
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(Qt::blue);
        painter.drawText(rect(), Qt::AlignCenter, m_text);
    }
private:
    QString m_text;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    DragDropWidget w;
    w.resize(400, 200);
    w.show();
    return app.exec();
}

#include "qt_drag_drop_event_example.moc"