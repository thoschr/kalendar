#include "view/monthview.h"
#include "persistence/securepmanager.h"
#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QPushButton>
#include <QHBoxLayout>
#include "test/test.h"
#include "util/linuxnotifymanager.h"
#include "util/eventutil.h"

#define RUN_TESTS 0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret = 0;
    #if RUN_TESTS
    Test t;
    t.test_persistence();
    t.test_util();
    #else
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
            {{"n", "notify"},
                QCoreApplication::translate("main", "Notify the events in the future <dayoffset> starting from today"),
                QCoreApplication::translate("main", "notifynextdays")},
            {{"a", "add"},
                QCoreApplication::translate("main", "Add an event"),
                QCoreApplication::translate("main", "event")},
            {{"d", "delete"},
                QCoreApplication::translate("main", "Delete an event"),
                QCoreApplication::translate("main", "event")},
        });
    parser.process(a);
    if (parser.isSet("add")) {
        QString event = parser.value("add");
        SecurePManager spm;
        spm.add_event(EventUtil::parseString(event.toStdString()));
    } else if(parser.isSet("delete")) {
        /* TODO: implement me */
        printf("Not implemented yet");
    }
    MonthView window;
    QString notify =  parser.value("notify");
    if (notify == "") {
        window.show();
        ret = a.exec();
    } else { /* Show notifications about the events in the next days */
        LinuxNotifyManager nm;
        if (!nm.notifyEvents(notify.toInt())) {
            printf("Error in notifyEvents");
            ret = 1;
        }
    }
    #endif
    return ret;
}
