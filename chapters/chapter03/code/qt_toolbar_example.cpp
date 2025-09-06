#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QLabel>

class ToolBarDemo : public QMainWindow
{
public:
    ToolBarDemo(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("QToolBar 示例");
        resize(500, 300);

        // 简单工具栏
        QToolBar *toolBar = addToolBar("工具栏");
        QAction *openAction = toolBar->addAction("打开");
        QAction *saveAction = toolBar->addAction("保存");

        // 中央窗口
        QLabel *label = new QLabel("中央窗口区域", this);
        label->setAlignment(Qt::AlignCenter);
        setCentralWidget(label);

        // 信号槽
        connect(openAction, &QAction::triggered, this, [=](){
            label->setText("点击了打开");
        });
        connect(saveAction, &QAction::triggered, this, [=](){
            label->setText("点击了保存");
        });
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ToolBarDemo w;
    w.show();
    return app.exec();
}
