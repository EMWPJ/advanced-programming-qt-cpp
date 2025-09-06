#include <QApplication>
#include <QWidget>
#include <QToolButton>
#include <QMenu>
#include <QHBoxLayout>
#include <QLabel>
class ToolButtonDemo : public QWidget
{
public:
ToolButtonDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QToolButton 示例");
        resize(480, 180);
        QLabel *label = new QLabel("请选择工具按钮操作", this);
        label->setAlignment(Qt::AlignCenter);
        QToolButton *toolBtn = new QToolButton(this);
        toolBtn->setText("工具按钮");
toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
toolBtn->setIcon(QIcon::fromTheme("document-open"));
        toolBtn->setAutoRaise(true);
        toolBtn->setCheckable(true);
        QMenu *menu = new QMenu(this);
        QAction *actionA = menu->addAction("操作A");
        QAction *actionB = menu->addAction("操作B");
        toolBtn->setMenu(menu);
toolBtn->setPopupMode(QToolButton::MenuButtonPopup);
        connect(toolBtn, &QToolButton::clicked,
        this, [=](bool checked){
        label->setText(checked ? "已按下" : "未按下"); });
        connect(actionA, &QAction::triggered, this,
            [=](){ label->setText("选择了操作A"); });
        connect(actionB, &QAction::triggered, this,
            [=](){ label->setText("选择了操作B"); });
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(toolBtn);
        layout->addWidget(label, 1);
        setLayout(layout);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ToolButtonDemo w;
    w.show();
    return app.exec();
}
