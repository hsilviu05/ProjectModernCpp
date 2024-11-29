#include "ProiectMCQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProiectMCQt w;
    w.show();
    return a.exec();
}
