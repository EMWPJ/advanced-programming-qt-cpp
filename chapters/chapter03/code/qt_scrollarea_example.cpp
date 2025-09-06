#include <QApplication>
#include <QScrollArea>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 创建内容部件，并设置合适的背景色以提升可读性
    QWidget *contentWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    // 批量添加标签，内容更丰富
    for (int i = 1; i <= 30; ++i) {
        QLabel *label = new QLabel(QString("这是第 %1 行内容 - QScrollArea 演示").arg(i));
        label->setMargin(4);
        layout->addWidget(label);
    }
    layout->addStretch(); // 增加弹性空间，优化滚动体验
    contentWidget->setLayout(layout);
    // 设置内容部件的最小宽度，避免内容过窄
    contentWidget->setMinimumWidth(220);
    // 创建 QScrollArea 并设置内容部件
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWindowTitle("QScrollArea 示例");
    scrollArea->resize(350, 250);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true); // 内容自适应滚动区域大小
    // 优化滚动条显示策略
scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    // 设置滚动区域背景色
    QPalette pal = scrollArea->palette();
    pal.setColor(QPalette::Window, QColor(245, 245, 250));
    scrollArea->setAutoFillBackground(true);
    scrollArea->setPalette(pal);
    scrollArea->show();
    return app.exec();
}
