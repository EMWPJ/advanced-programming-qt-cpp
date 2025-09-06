#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

class StatusBarDemo : public QMainWindow
{
public:
    StatusBarDemo(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("QStatusBar 示例");
        resize(480, 220);
        // 创建状态栏
        QStatusBar *statusBar = new QStatusBar(this);
        setStatusBar(statusBar);
        // 显示初始信息
        statusBar->showMessage("欢迎使用 QStatusBar 示例", 3000);
        // 添加永久信息标签
        QLabel *permanentLabel = new QLabel("就绪", this);
        statusBar->addPermanentWidget(permanentLabel);
        // 添加临时信息按钮
        QPushButton *infoBtn = new QPushButton("显示临时信息", this);
        statusBar->addWidget(infoBtn);
        // 信号槽：点击按钮显示临时信息
        connect(infoBtn, &QPushButton::clicked, this, [=](){
            statusBar->showMessage("这是临时信息，3秒后消失", 3000);
        });
        // 中央窗口区域
        QLabel *centerLabel = new QLabel("主窗口内容区域", this);
        centerLabel->setAlignment(Qt::AlignCenter);
        setCentralWidget(centerLabel);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StatusBarDemo w;
    w.show();
    return app.exec();
}
