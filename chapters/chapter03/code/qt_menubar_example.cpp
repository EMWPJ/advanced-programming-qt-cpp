#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>

class MenuBarDemo : public QMainWindow
{
public:
    MenuBarDemo(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setWindowTitle("QMenuBar 示例");
        resize(500, 280);
        QMenuBar *menuBar = new QMenuBar(this);
        setMenuBar(menuBar);
        QMenu *fileMenu = menuBar->addMenu("文件(&F)");
        QAction *openAction = fileMenu->addAction("打开(&O)");
        QAction *exitAction = fileMenu->addAction("退出(&Q)");
        QMenu *helpMenu = menuBar->addMenu("帮助(&H)");
        QAction *aboutAction = helpMenu->addAction("关于(&A)");
        QLabel *label = new QLabel("请选择菜单栏操作", this);
        label->setAlignment(Qt::AlignCenter);
        setCentralWidget(label);
        connect(openAction, &QAction::triggered,
        this, [=](){ label->setText("打开"); });
        connect(exitAction, &QAction::triggered,
        this, [=](){ close(); });
        connect(aboutAction, &QAction::triggered,
        this, [=](){ label->setText("QMenuBar"); });
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MenuBarDemo w;
    w.show();
    return app.exec();
}
