#include <QApplication>
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
class SliderDemo : public QWidget {
public:
SliderDemo(QWidget *parent = nullptr) : QWidget(parent) {
    setWindowTitle("QSlider 示例");
    resize(400, 180);
    auto *label = new QLabel("当前值：50", this);
    label->setAlignment(Qt::AlignCenter);
    auto *hSlider = new QSlider(Qt::Horizontal, this);
    auto *vSlider = new QSlider(Qt::Vertical, this);
    hSlider->setRange(0, 100);  vSlider->setRange(0, 100);
    hSlider->setValue(50);  vSlider->setValue(50);
    hSlider->setTickInterval(10);
    vSlider->setTickInterval(10);
    hSlider->setTickPosition(QSlider::TicksBelow);
    vSlider->setTickPosition(QSlider::TicksLeft);
    // 只需一个信号槽即可同步两个滑块
    connect(hSlider, &QSlider::valueChanged, vSlider, &QSlider::setValue);
    connect(vSlider, &QSlider::valueChanged, hSlider, &QSlider::setValue);
    connect(hSlider, &QSlider::valueChanged, label, [=](int value){
    label->setText(QString("当前值：%1").arg(value)); });
    connect(vSlider, &QSlider::valueChanged, label, [=](int value){
    label->setText(QString("当前值：%1").arg(value)); });
    auto *hLayout = new QHBoxLayout;
    hLayout->addWidget(vSlider);
    hLayout->addWidget(hSlider, 1);
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(label);
    setLayout(mainLayout);
}};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SliderDemo w;
    w.show();
    return app.exec();
}
