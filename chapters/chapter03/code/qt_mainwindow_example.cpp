#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QLabel>
#include <QDebug>

class MyMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MyMainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("QMainWindow 示例");
        resize(600, 400);

        // 创建菜单栏
        QMenuBar *menuBar = new QMenuBar(this);
        setMenuBar(menuBar);

        // 文件菜单
        QMenu *fileMenu = menuBar->addMenu("文件");
        QAction *openAction = fileMenu->addAction("打开");
        QAction *exitAction = fileMenu->addAction("退出");

        // 编辑菜单
        QMenu *editMenu = menuBar->addMenu("编辑");
        QAction *copyAction = editMenu->addAction("复制");
        QAction *pasteAction = editMenu->addAction("粘贴");

        // 工具栏
        QToolBar *toolBar = addToolBar("主工具栏");
        toolBar->addAction(openAction);
        toolBar->addAction(copyAction);

        // 状态栏
        QStatusBar *statusBar = new QStatusBar(this);
        setStatusBar(statusBar);
        statusBar->showMessage("欢迎使用 QMainWindow 示例");

        // 中央窗口区域
        QLabel *label = new QLabel("这是中央窗口区域", this);
        label->setAlignment(Qt::AlignCenter);
        setCentralWidget(label);

        // 信号槽连接
        connect(openAction, &QAction::triggered, this, [=](){
            statusBar->showMessage("点击了打开");
            qDebug() << "打开文件";
        });
        connect(exitAction, &QAction::triggered, this, [=](){
            close();
        });
        connect(copyAction, &QAction::triggered, this, [=](){
            statusBar->showMessage("点击了复制");
            qDebug() << "复制";
        });
        connect(pasteAction, &QAction::triggered, this, [=](){
            statusBar->showMessage("点击了粘贴");
            qDebug() << "粘贴";
        });
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyMainWindow w;
    w.show();
    return app.exec();
}

#include "qt_mainwindow_example.moc"