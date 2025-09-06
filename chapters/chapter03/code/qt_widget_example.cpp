#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

class WidgetDemo : public QWidget
{
public:
    WidgetDemo(QWidget *parent = nullptr) : QWidget(parent) {
        // 初始化设置
        setWindowTitle("QWidget 示例演示");  // 设置窗口标题
        resize(400, 300);  // 设置窗口大小
        setMinimumSize(200, 150);  // 设置窗口最小大小
        setMaximumSize(800, 600);  // 设置窗口最大大小
        // 窗口属性设置
        setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);  // 设置窗口标志
        setStyleSheet("background-color: lightgray;");  // 设置窗口样式
        setMouseTracking(true);  // 设置鼠标跟踪
        setCursor(Qt::ArrowCursor);  // 设置鼠标形状
        setAcceptDrops(true);  // 设置窗口接受拖放
        setAutoFillBackground(true);  // 设置窗口自动填充背景
        setFocusPolicy(Qt::StrongFocus);  // 设置窗口焦点策略
        m_timerId = startTimer(1000);  // 启动定时器，每秒触发一次
        m_step = 10;  // 设置步长
        m_colorIndex = 0;  // 设置颜色索引
    }

protected:
    // 事件处理函数
    void timerEvent(QTimerEvent *event) override {
        qDebug() << "定时器事件处理";
        setStyleSheet("background-color: " + m_colors[m_colorIndex] + ";");
        m_colorIndex = (m_colorIndex + 1) % 3;
    }
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *event) override {
        qDebug() << "鼠标点击位置:" << event->pos();
        m_colorIndex = (m_colorIndex + 1) % 3;
    }
    // 按键事件
    void keyPressEvent(QKeyEvent *event) override {
        qDebug() << "按键事件:" << event->text();
        const int step = 10;
        switch(event->key()) {
            case Qt::Key_Up:
                move(pos() - QPoint(0, step));  // 上移
                break;
            case Qt::Key_Down:
                move(pos() + QPoint(0, step));  // 下移
                break;
            case Qt::Key_Left:
                move(pos() - QPoint(step, 0));  // 左移
                break;
            case Qt::Key_Right:
                move(pos() + QPoint(step, 0));  // 右移
                break;
            case Qt::Key_Space:
                resize(size() + QSize(step, step));  // 放大
                break;
            case Qt::Key_Control:
                resize(size() - QSize(step, step));  // 缩小
                break;
            default:
                QWidget::keyPressEvent(event);
        }
    }
    // 拖放事件
    void dragEnterEvent(QDragEnterEvent *event) override {
        qDebug() << "拖放事件:" << event->mimeData()->text();
    }
    // 拖放事件
    void dropEvent(QDropEvent *event) override {
        qDebug() << "拖放事件:" << event->mimeData()->text();
    }

private:
    int m_timerId;  // 用于存储定时器ID，以便后续停止
    int m_step;  // 用于存储步长
    int m_colorIndex;  // 用于存储颜色索引
    const QString m_colors[3] = {"red", "blue", "green"};  // 用于存储颜色
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    WidgetDemo mainWindow1;
    mainWindow1.show();
    WidgetDemo childWindow(&mainWindow1);
    childWindow.setWindowTitle("子窗口1");
    childWindow.resize(200, 150);
    childWindow.show();
    // 坐标转换示例
    QPoint globalPos1 = mainWindow1.mapToGlobal(QPoint(1001, 1001));
    QPoint localPos1 = mainWindow1.mapFromGlobal(globalPos1);
    qDebug() << "坐标转换示例 - 全局:" << globalPos1 << " 局部:" << localPos1;
    return app.exec();
}
