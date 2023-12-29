#include "myplot.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPlate w;
    w.show();
    return a.exec();
}
