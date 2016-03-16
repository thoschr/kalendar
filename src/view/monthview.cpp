#include "monthview.h"
#include "ui_monthview.h"

#include "../util/timeutil.h"

#include <QDebug>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border:none; }" \
                   "QFrame#today { background-color: #FFFF66; }"

/* This is just a draft to get an idea */

MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    Time time = TimeUtil::get_current_time();
    int tot_days = TimeUtil::get_days_in_month(time.getMonth(), time.getYear());
    QLabel *label_date = new QLabel(QString(TimeUtil::get_literal_month(time.getMonth()).c_str()) + QString(" ") + QString::number(time.getYear()));
    QGridLayout *grid_layout = new QGridLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    label_date->setMaximumHeight(20);
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
            frame->setMinimumWidth(60);
            frame->setMinimumHeight(60);
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

MonthView::~MonthView()
{
    delete ui;
}
