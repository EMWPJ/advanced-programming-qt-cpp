#include <QApplication>
#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // 创建主窗口部件
    QWidget window;
    window.setWindowTitle("QSplitter 示例");
    window.resize(500, 300);
    // 创建水平分割器
    QSplitter *splitter = new QSplitter(Qt::Horizontal, &window);
    // 左侧控件：QTextEdit
    QTextEdit *editor = new QTextEdit;
    editor->setPlainText("左侧：QTextEdit\n可以输入多行文本。");
    // 右侧控件：QFrame + QLabel
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    QLabel *label = new QLabel("右侧：QLabel\n可放置任意控件。");
    label->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(label);
    // 添加控件到分割器
    splitter->addWidget(editor);
    splitter->addWidget(frame);
    // 设置初始分割比例
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->addWidget(splitter);
    window.show();
    return app.exec();
}
