#include <QApplication>
#include <QWidget>
#include <QScrollBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
class ScrollBarDemo : public QWidget {
public:
ScrollBarDemo(QWidget *parent = nullptr) : QWidget(parent) {
setWindowTitle("QScrollBar 示例");
resize(400, 180);
auto *label = new QLabel("当前值：50", this);
label->setAlignment(Qt::AlignCenter);
auto *hScrollBar = new QScrollBar(Qt::Horizontal, this);
auto *vScrollBar = new QScrollBar(Qt::Vertical, this);
hScrollBar->setRange(0, 100); vScrollBar->setRange(0, 100);
hScrollBar->setValue(50); vScrollBar->setValue(50);
hScrollBar->setPageStep(10); vScrollBar->setPageStep(10);
connect(hScrollBar, &QScrollBar::valueChanged, vScrollBar, &QScrollBar::setValue);
connect(vScrollBar, &QScrollBar::valueChanged, hScrollBar, &QScrollBar::setValue);
connect(hScrollBar, &QScrollBar::valueChanged, label, [=](int v){ label->setText(QString("当前值：%1").arg(v)); });
connect(vScrollBar, &QScrollBar::valueChanged, label, [=](int v){ label->setText(QString("当前值：%1").arg(v)); });
auto *hLayout = new QHBoxLayout;
hLayout->addWidget(vScrollBar);
hLayout->addWidget(hScrollBar, 1);
auto *mainLayout = new QVBoxLayout(this);
mainLayout->addLayout(hLayout);
mainLayout->addWidget(label);
setLayout(mainLayout);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ScrollBarDemo w;
    w.show();
    return app.exec();
}
