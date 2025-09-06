#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
class MyWindow : public QWidget {
    Q_OBJECT
public:
    MyWindow(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        label = new QLabel(QStringLiteral("你好，Qt OOP GUI 示例！"), this);
        QPushButton* button = new QPushButton(QStringLiteral("点击我"), this);
        layout->addWidget(label);
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, this, &MyWindow::onButtonClicked);
    }
private slots:
    void onButtonClicked() { label->setText(QStringLiteral("按钮已被点击！")); }
private:
    QLabel* label;
};
#include "oop_example_gui.moc"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyWindow window;
    window.setWindowTitle(QStringLiteral("OOP GUI 示例"));
    window.resize(300, 120);
    window.show();
    return app.exec();
}
