#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QMessageBox>
class MenuDemo : public QMainWindow
{
public:
    MenuDemo(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("QMenu 示例");
        resize(480, 260);
        // 创建菜单栏
        QMenuBar *menuBar = new QMenuBar(this);
        setMenuBar(menuBar);
        // 文件菜单
        QMenu *fileMenu = menuBar->addMenu("文件(&F)");
        QAction *newAction = fileMenu->addAction("新建(&N)");
        fileMenu->addSeparator();
        QAction *exitAction = fileMenu->addAction("退出(&Q)");
        // 编辑菜单
        QMenu *editMenu = menuBar->addMenu("编辑(&E)");
        QAction *copyAction = editMenu->addAction("复制(&C)");
        // 中央窗口
        QLabel *label = new QLabel("请选择菜单项进行操作", this);
        label->setAlignment(Qt::AlignCenter);
        setCentralWidget(label);
        // 信号槽连接
        connect(newAction, &QAction::triggered,
        this, [=](){ label->setText("新建操作"); });
        connect(exitAction, &QAction::triggered,
        this, [=](){ close(); });
        connect(copyAction, &QAction::triggered,
        this, [=](){ label->setText("复制操作"); });
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MenuDemo w;
    w.show();
    return app.exec();
}
