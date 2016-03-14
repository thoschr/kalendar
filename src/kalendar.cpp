#include "kalendar.h"
#include "ui_kalendar.h"
#include "util/timeutil.h"

#include <QDebug>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; height: 100px; width: 100px; }" \
                   "QLabel { border:none; }" \
                   "QFrame#today { background-color: #FFFF66; }"

/* This is just a draft to get an idea */

Kalendar::Kalendar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kalendar)
{
    Time time = TimeUtil::get_current_time();
    int tot_days = TimeUtil::get_days_in_month(time.getMonth(), time.getYear());
    QLabel *label_date = new QLabel(QString(TimeUtil::get_literal_month(time.getMonth()).c_str()) + QString(" ") + QString::number(time.getYear()));
    QGridLayout *grid_layout = new QGridLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label_date);
    //Create 7x7 grid
    int i,j,x;
    x = 1;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            QFrame *frame = new QFrame;
            QVBoxLayout *vl = new QVBoxLayout;
            if (((i > 0) || (j >= time.getWeekDay()-1)) && (x <= tot_days)) {
                if (x == time.getMonthDay())
                    frame->setObjectName("today");
                QLabel *day = new QLabel(QString::number(x));
                vl->addWidget(day);
                x++;
            }
            frame->setLayout(vl);
            frame->setStyleSheet(CELL_STYLE);
            grid_layout->addWidget(frame, i, j);
        }
    }
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);

    layout->addLayout(grid_layout);

     // Set layout in QWidget
     QWidget *window = new QWidget();
     window->setLayout(layout);

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

Kalendar::~Kalendar()
{
    delete ui;
}
