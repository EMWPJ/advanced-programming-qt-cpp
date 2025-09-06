#include <QApplication>
#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLabel>

class TreeWidgetDemo : public QWidget
{
public:
    TreeWidgetDemo(QWidget *parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("QTreeWidget 示例");
        resize(420, 300);
        auto *label = new QLabel("请选择节点", this);
        label->setAlignment(Qt::AlignCenter);
        auto *tree = new QTreeWidget(this);
        tree->setColumnCount(2);
        tree->setHeaderLabels(QStringList() << "名称" << "描述");
        auto *root1 = new QTreeWidgetItem(tree, QStringList() << "水果" << "各种水果");
        new QTreeWidgetItem(root1, QStringList() << "苹果" << "红色或绿色");
        new QTreeWidgetItem(root1, QStringList() << "香蕉" << "黄色，富含钾");
        auto *root2 = new QTreeWidgetItem(tree, QStringList() << "蔬菜" << "各种蔬菜");
        new QTreeWidgetItem(root2, QStringList() << "西红柿" << "红色，富含维生素C");
        new QTreeWidgetItem(root2, QStringList() << "黄瓜" << "绿色，清爽可口");
        tree->expandAll();
        connect(tree, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem *item, int){
            label->setText(QString("已选择：%1 - %2").arg(item->text(0), item->text(1)));
        });
        auto *vbox = new QVBoxLayout(this);
        vbox->addWidget(tree);
        vbox->addWidget(label, 1);
        setLayout(vbox);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TreeWidgetDemo w;
    w.show();
    return app.exec();
}
