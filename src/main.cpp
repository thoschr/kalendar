#include "view/monthview.h"
#include "persistence/pmanager.h"
#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QPushButton>
#include <QHBoxLayout>
#include "test/test.h"
#include "util/linuxnotifymanager.h"

#define RUN_TESTS 0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #if RUN_TESTS
    Test t;
    //t.test_persistence();
    t.test_util();
    #else
    MonthView window;
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
            {{"n", "notify"},
                QCoreApplication::translate("main", "Notify the events in the future <dayoffset> starting from today"),
                QCoreApplication::translate("main", "notifynextdays")},
        });
    parser.process(a);
    QString notify =  parser.value("notify");
    if (notify == "") {
        window.show();
    } else { /* Show notifications about the events in the next days */
        LinuxNotifyManager nm;
        nm.notifyEvents(notify.toInt());
    }
    #endif
    return a.exec();
}
