#include "monthview.h"
#include "ui_monthview.h"

#include "../util/dateutil.h"

#include "qframe_extended.h"

#include <QDebug>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; }" \
                   "QFrame#day:hover { background-color: #EEEEFF; }" \
                   "QLabel#header { background-color: #EEEEEE; padding: auto; font-weight: bold; }"


void MonthView::on_mouse_press(QFrameExtended *frame) {
    this->selection_start = frame->getDate();
}

void MonthView::on_mouse_release(QFrameExtended *frame) {
    this->selection_end = frame->getDate();
    //TODO: Show the window to add an event with the start and the end already setted
}

void MonthView::on_back_button_click() {
    /* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted. */
    display_days(DateUtil::decrease_month(this->frames[21]->getDate()));
}

void MonthView::on_next_button_click() {
    /* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted. */
    display_days(DateUtil::increase_month(this->frames[21]->getDate()));
}

MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    Date current_date = DateUtil::get_current_date();

    this->label_date = new QLabel;
    this->layout = new QVBoxLayout;
    label_date->setMaximumHeight(20);
    label_date->setStyleSheet("QLabel { padding-left: 100px; padding-right: 100px; font-size: 20px; } ");
    QPushButton *back = new QPushButton("<");
    QPushButton *next = new QPushButton(">");
    back->setMaximumWidth(60);
    next->setMaximumWidth(60);
    connect(back, &QPushButton::clicked, this, &MonthView::on_back_button_click);
    connect(next, &QPushButton::clicked, this, &MonthView::on_next_button_click);
    QHBoxLayout *hl = new QHBoxLayout;
    hl->setAlignment(Qt::AlignCenter);
    hl->addWidget(back);
    hl->addWidget(label_date);
    hl->addWidget(next);
    //TODO map the keyboard arrows to the handler on_back_.. and on_next_..
    this->layout->addLayout(hl);

    //Create 7x7 grid
    QGridLayout *grid_layout = new QGridLayout;
    int i,j;
    //First row contains the names of the days of the week
    for (j = 0; j < 7; j++) {
        QFrame *frame = new QFrame;
        QVBoxLayout *vl = new QVBoxLayout;
        frame->setMinimumWidth(150);
        frame->setMinimumHeight(100);
        QLabel *wday_name = new QLabel(DateUtil::numeric2literal_day_of_week(j+1).c_str());
        wday_name->setObjectName("header");
        vl->addWidget(wday_name);
        vl->setMargin(0);
        frame->setLayout(vl);
        frame->setStyleSheet(CELL_STYLE);
        grid_layout->addWidget(frame, 0, j);
    }

    //Next rows contain the days of the selected month
    for (i = 1; i < 7; i++) { //rows
        for (j = 0; j < 7; j++) { //columns
            //Map a Time object to each frame
            QFrameExtended *frame = new QFrameExtended;
            QVBoxLayout *vl = new QVBoxLayout;
            QLabel *day = new QLabel;
            frame->setObjectName("day");
            day->setMaximumHeight(25);
            vl->setAlignment(Qt::AlignTop | Qt::AlignRight);
            vl->addWidget(day);
            vl->setMargin(0);
            connect(frame, &QFrameExtended::pressed, this, &MonthView::on_mouse_press);
            connect(frame, &QFrameExtended::released, this, &MonthView::on_mouse_release);
            frame->setMinimumWidth(150);
            frame->setMinimumHeight(100);
            frame->setLayout(vl);
            frame->setStyleSheet(CELL_STYLE);
            grid_layout->addWidget(frame, i, j);
            this->frames[7*(i-1)+j] = frame;
        }
    }
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);

    //Fill the grid with the days of the default month (i.e. the current month)
    display_days(current_date);

    this->layout->addLayout(grid_layout);

     // Set layout in QWidget
     QWidget *window = new QWidget();
     window->setLayout(this->layout);
     window->setMinimumHeight(700);
     window->setMinimumWidth(1200);

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

MonthView::~MonthView()
{
    delete ui; //seems this frees also this->frames
    delete this->layout;
}

void MonthView::display_days(Date date) { //TODO clean today cell
    //Update the label that contains month and year
    this->label_date->setText(QString(DateUtil::get_literal_month(date.getMonth()).c_str()) + QString("    ") + QString::number(date.getYear()));
    //The current time is needed to highlight the current day
    Date current_date = DateUtil::get_current_date();
    int tot_days = DateUtil::get_days_in_month(date.getMonth(), date.getYear());
    int i,x,start_wday;

    //first week day of the current month
    start_wday = DateUtil::get_first_day_of_month(date).getWeekDay();
    x = 1;
    for (i = 0; i < 42; i++) {
        this->frames[i]->setDate(Date(x, (start_wday + (x-1)) % 7, date.getMonth(), date.getYear()));
        QLabel *day = static_cast<QLabel*> (this->frames[i]->children().at(1));
        //First clean and then overwrite
        day->setObjectName("");
        day->setText("");
        //Checks right cells that will contain the days
        if (( i > 6 || //if I'm after the first week or
            (i % 7 >= start_wday-1)) && //if I'm in the first week and I'm in the right days
            (x <= tot_days)) { //and I'm not out of bound
            //I'll insert into the label the number of the day
            day->setText(QString::number(x));
            //Checks current day
            if ((x == current_date.getMonthDay()) && (date.getMonth() == current_date.getMonth()) && (date.getYear() == current_date.getYear()))
                day->setObjectName("today");
            x++;
        }
        //Refresh the css rules
        this->frames[i]->setStyleSheet(CELL_STYLE);
    }
}
