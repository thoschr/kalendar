#include "kalendar.h"
#include "persistence/pmanager.h"
#include <QApplication>
#include "test/test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //kalendar w;
    //w.show();
    //PManager pm;
    Test t;
    t.test_persistence();
    return a.exec();
}
