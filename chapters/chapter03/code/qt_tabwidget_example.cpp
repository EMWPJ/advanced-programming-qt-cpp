#include <QApplication>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTextEdit>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 创建主窗口部件
    QWidget window;
    window.setWindowTitle("QTabWidget 示例");
    window.resize(420, 260);
    // 创建 QTabWidget
    QTabWidget *tabWidget = new QTabWidget(&window);
    // 第一个选项卡：简单标签
    QWidget *tab1 = new QWidget;
    QVBoxLayout *tab1Layout = new QVBoxLayout(tab1);
    QLabel *label1 = new QLabel("这是第一个选项卡内容。");
    tab1Layout->addWidget(label1);
    tab1Layout->addStretch();
    // 第二个选项卡：文本编辑器
    QWidget *tab2 = new QWidget;
    QVBoxLayout *tab2Layout = new QVBoxLayout(tab2);
    QTextEdit *textEdit = new QTextEdit;
    textEdit->setPlainText("可以在这里输入多行文本。");
    tab2Layout->addWidget(textEdit);
    // 添加选项卡到 QTabWidget
    tabWidget->addTab(tab1, "选项卡一");
    tabWidget->addTab(tab2, "选项卡二");
    // 设置选项卡可关闭
    tabWidget->setTabsClosable(true);
    QObject::connect(tabWidget, &QTabWidget::tabCloseRequested, tabWidget, &QTabWidget::removeTab);
    // 设置选项卡位置（可选：顶部、底部、左侧、右侧）
    tabWidget->setTabPosition(QTabWidget::North);
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->addWidget(tabWidget);
    window.show();
    return app.exec();
}
