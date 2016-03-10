#include "kalendar.h"
#include "persistence/pmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //kalendar w;
    //w.show();
    PManager pm;

    return a.exec();
}
