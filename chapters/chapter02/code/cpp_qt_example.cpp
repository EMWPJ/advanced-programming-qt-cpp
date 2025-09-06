#include <QApplication>
#include <QLabel>
#include <windows.h>

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(65001);

    QApplication app(argc, argv);

    QLabel *label = new QLabel("Hello Qt!");
    label->setWindowTitle("Qt Example");
    label->resize(200, 100);
    label->show();

    return app.exec();
}