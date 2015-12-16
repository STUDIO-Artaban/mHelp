#include "AppQt.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppQt w;
    w.show();
    return a.exec();
}
