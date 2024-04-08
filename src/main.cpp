#include "onlinemap.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OnlineMap w;
    w.show();
    return a.exec();
}
