QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
    test.cpp\
    ../persistence/pmanager.cpp \
    ../tools/sqlite3/sqlite3.c \
    persistence/pmanager_test.cpp \
    ../util/dateutil.cpp \
    util/dateutil_test.cpp \
    ../util/eventutil.cpp \
    util/eventutil_test.cpp \

HEADERS  += ../tools/sqlite3/sqlite3.h \
    ../persistence/pmanager.h \
    test.h \
    persistence/pmanager_test.h \
    ../util/dateutil.h \
    util/dateutil_test.h \
    ../util/eventutil.h \
    util/eventutil_test.h \

QMAKE_LIBS += -lstdc++fs
QMAKE_CXXFLAGS += -std=c++17

#For DEBUG purpose
#QMAKE_CXXFLAGS += -g
