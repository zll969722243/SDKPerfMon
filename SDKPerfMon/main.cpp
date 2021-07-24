#include "SDKPerfMon.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SDKPerfMon w;
    w.show();
    w.Init();

    return a.exec();
}
