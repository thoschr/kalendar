#include "monthview.h"
#include "ui_monthview.h"

#include "../util/dateutil.h"
#include "eventdialog.h"
#include "qframe_extended.h"
#include "qwidget_extended.h"

#include <QDebug>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; }" \
                   "QFrame#selected { background-color: #EEEEFF; }" \
                   "QLabel#header { background-color: #EEEEEE; padding: auto; font-weight: bold; }"

void MonthView::on_mouse_move(QFrameExtended *frame) {
    if ((frame->getDate() != NULL) && //Checks if the frame is valid
        (this->selection_start != NULL) && //and if the selection is already started
        (!this->selection_end || frame->getDate()->compareTo(*this->selection_end) != 0)) { //and if this is a new cell
        this->selection_end = frame->getDate(); //Keeps in memory where we're arrived
        for (int i = 0; i < 42; i++) {
            if (this->frames[i]->getDate() != NULL) {
                if ((this->frames[i]->getDate()->compareTo(*this->selection_start) >= 0) &&
                    (this->frames[i]->getDate()->compareTo(*frame->getDate()) <= 0)) {
                    this->frames[i]->setObjectName("selected");
                    this->frames[i]->setStyleSheet(CELL_STYLE);
                } else {
                    this->frames[i]->setObjectName("day");
                    this->frames[i]->setStyleSheet(CELL_STYLE);
                }
            }
        }
    }
}

void MonthView::on_mouse_press(QFrameExtended *frame) {
    if (frame->getDate() != NULL) //Checks if the frame is valid
        this->selection_start = frame->getDate();
}

void MonthView::on_mouse_release(QFrameExtended *frame) {
    if ((frame->getDate() != NULL) && //Checks if the frame is valid
        (this->selection_start != NULL)) { //and if the selection is already started
        this->selection_end = frame->getDate();
        /* Clean the selection */
        for (int i = 0; i < 42; i++) {
            if (this->frames[i]->getDate() != NULL) {
                this->frames[i]->setObjectName("day");
                this->frames[i]->setStyleSheet(CELL_STYLE);
            }
        }
        /* The following lines will popup the dialog to add an event with the start and the end already setted,
         * but only if the user selects a valid range.
         */
        if (this->selection_end->compareTo(*this->selection_start) >= 0) {
            EventDialog *eventDialog = new EventDialog(*this->selection_start, *this->selection_end);
            eventDialog->show();
        }
        this->selection_start = NULL;
    }
}

void MonthView::on_back_button_click() {
    /* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted. */
    Date newDate = DateUtil::decrease_month(*this->frames[21]->getDate());
    display_days(newDate);
    /* Reload events */
    display_events(newDate);
}

void MonthView::on_next_button_click() {
    /* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted. */
    Date newDate = DateUtil::increase_month(*this->frames[21]->getDate());
    display_days(newDate);
    /* Reload events */
    display_events(newDate);
}

MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    Date current_date = DateUtil::get_current_date();
    this->selection_start = NULL;
    this->selection_end = NULL;
    this->label_date = new QLabel;
    this->layout = new QVBoxLayout;
    this->label_date->setMaximumHeight(40);
    this->label_date->setStyleSheet("QLabel { padding-left: 100px; padding-right: 100px; font-size: 20px; } ");
    QPushButton *back = new QPushButton("<");
    QPushButton *next = new QPushButton(">");
    back->setMaximumWidth(60);
    next->setMaximumWidth(60);
    back->setMaximumHeight(40);
    next->setMaximumHeight(40);
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
            frame->setDate(NULL);
            frame->setObjectName("day");
            vl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
            vl->setMargin(0);
            vl->addWidget(new QLabel);
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

    //Load the events for the current month
    display_events(current_date);

    grid_layout->setMargin(5);
    this->layout->addLayout(grid_layout);

     // Set layout in QWidget
     QWidgetExtended *window = new QWidgetExtended;
     window->setLayout(this->layout);
     window->setMinimumHeight(600);
     window->setMinimumWidth(1100);

     connect(window, &QWidgetExtended::mousePress, this, &MonthView::on_mouse_press);
     connect(window, &QWidgetExtended::mouseRelease, this, &MonthView::on_mouse_release);
     connect(window, &QWidgetExtended::mouseMove, this, &MonthView::on_mouse_move);

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

MonthView::~MonthView()
{
    delete ui; //seems this frees also this->frames
    delete this->layout;
    delete this->selection_start;
    delete this->selection_end;
}

void MonthView::display_days(Date date) {
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
        //Set an invalid date
        if (this->frames[i]->getDate() != NULL) {
            delete this->frames[i]->getDate();
            this->frames[i]->setDate(NULL);
        }
        //Delete all the event labels inside the frame
        QLabelEvent label_event;
        QListIterator<QObject *> it (this->frames[i]->children());
        while (it.hasNext()) {
            QObject *o = qobject_cast<QObject*> (it.next());
            if (o->metaObject()->className() == label_event.metaObject()->className()){
                delete o;
            }
        }
        QLabel *day = static_cast<QLabel*> (this->frames[i]->children().at(1));
        day->setObjectName("");
        day->setText("");
        //Checks right cells that will contain the days
        if (( i > 6 || //if I'm after the first week or
            (i % 7 >= start_wday-1)) && //if I'm in the first week and I'm in the right days
            (x <= tot_days)) { //and I'm not out of bound
            this->frames[i]->setDate(new Date(x, (start_wday + (x-1)) % 7, date.getMonth(), date.getYear()));
            //I'll insert into the label the number of the day
            day->setText(QString::number(x));
            //Checks current day
            if ((x == current_date.getMonthDay()) && (date.getMonth() == current_date.getMonth()) && (date.getYear() == current_date.getYear()))
                day->setObjectName("today");
            this->frames[i]->layout()->addWidget(day);
            x++;
        }
        //Refresh the css rules
        this->frames[i]->setStyleSheet(CELL_STYLE);
    }
}

void MonthView::display_events(Date date) {
    PManager pm;
    list<Event*> event_list = pm.get_events_of_month(date.getMonth(), date.getYear());
    int start_offset;
    //Find at which cell the month starts
    for (start_offset = 0; start_offset < 42; start_offset++) {
        //Looks where is the first day of the month
        if (this->frames[start_offset]->getDate() != NULL) break;
    }
    //Add events to the gui
    for (Event *event : event_list) {
        Date start = DateUtil::date_from_timestamp(event->getStart());
        Date end = DateUtil::date_from_timestamp(event->getEnd());
        QLabelEvent *label_event = new QLabelEvent;
        label_event->setText(event->getName().c_str());
        label_event->setEvent(event);
        QString textColor("#000000");
        if (is_color_dark(event->getCategory()->getColor()))
            textColor = "#FFFFFF";
        label_event->setStyleSheet(QString("QLabel { background-color : ") + QString(event->getCategory()->getColor().c_str()) + QString("; color: ") + textColor + QString("};"));
        label_event->setMinimumWidth(this->frames[start_offset+start.getMonthDay()-1]->minimumWidth());
        label_event->setToolTip(event->getDescription().c_str());
        this->frames[start_offset+start.getMonthDay()-1]->layout()->addWidget(label_event);
        //TODO: display label from start to end date
        connect(label_event, &QLabelEvent::clicked, this, &MonthView::on_event_click);
    }
}

void MonthView::display_events(Date date, Category category) {

}

bool MonthView::is_color_dark(string colorName) {
    QColor color(colorName.c_str());
    //Formula to calculate luminance from ITU-R BT.709
    int l = 0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue();
    if (l < 40)
        return true;
    else
        return false;
}

void MonthView::on_event_click(Event *event) {
    EventDialog *eventDialog = new EventDialog(DateUtil::date_from_timestamp(event->getStart()), DateUtil::date_from_timestamp(event->getEnd()), event->getName(), event->getDescription(), event->getCategory()->getName());
    eventDialog->show();
}

//FIXME
/* BUG:
 * When the user changes the month, opens the EventDialog window, closes it and then changes the month again, when he'll close
 * the program, will be the following error: corrupted double-linked list
 */
