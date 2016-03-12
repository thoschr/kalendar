#include "kalendar.h"
#include "persistence/pmanager.h"
#include <QApplication>
#include <QDebug>
#include "test/test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //kalendar w;
    //w.show();
    //PManager pm;
    Test t;
    t.test_persistence();
    qDebug() << "end" << endl;
    return a.exec();
}
